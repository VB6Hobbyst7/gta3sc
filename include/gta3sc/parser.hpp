#include <gta3sc/scanner.hpp>
#include <gta3sc/arena-allocator.hpp>
#include <variant>

#include <algorithm>
#include <cstring>

namespace gta3sc
{
/// This is an intermediate representation for syntactically
/// valid GTA3script.
/// 
/// The implication of this being syntatically valid is that e.g.
/// for every WHILE command, there is a matching ENDWHILE one.
///
/// Structured statements such as IF and WHILE also become IR (they
/// are commands after all) where their first argument determines how
/// many of the following instructions are part of the conditional
/// list, much like the unstructured ANDOR command.
///
/// This IR preserves source code information such as the location of
/// each of its identifiers.
struct ParserIR
{
    struct SourceInfo
    {
        const SourceFile& source;
        SourceRange range;
    };


    struct Identifier { std::string_view name; };

    struct Filename { std::string_view filename; };

    struct String { std::string_view string; };

    struct Argument
    {
        arena_ptr<SourceInfo> source_info;
        std::variant<
            int32_t,
            float,
            Identifier,
            Filename,
            String> value;
    };


    struct LabelDef
    {
        arena_ptr<SourceInfo> source_info;
        std::string_view name;
    };

    struct Command
    {
        arena_ptr<SourceInfo> source_info;
        std::string_view name;
        arena_ptr<arena_ptr<Argument>> arguments = nullptr;
        size_t num_arguments = 0;
    };

    std::variant<LabelDef, Command> op;
    arena_ptr<ParserIR> next = nullptr;
    arena_ptr<ParserIR> prev = nullptr;

    // use set_next for set_prev behaviour
    void set_next(arena_ptr<ParserIR> other)
    {
        assert(other != nullptr);

        if(this->next)
        {
            assert(this->next->prev == this);
            this->next->prev = nullptr;
        }

        this->next = other;

        if(other->prev)
        {
            assert(other->prev->next == other);
            other->prev->next = nullptr;
        }

        other->prev = this;
    }


    static auto create_source_info(const SourceInfo& info, ArenaMemoryResource& arena) -> arena_ptr<SourceInfo>
    {
        return new (arena, alignof(SourceInfo)) SourceInfo(info);
    }

    static auto create_command(const SourceInfo& info, std::string_view name_a, ArenaMemoryResource& arena) -> arena_ptr<ParserIR>
    {
        auto node = new (arena, alignof(ParserIR)) ParserIR;
        auto name = create_upper_view(name_a, arena);
        node->op = Command { create_source_info(info, arena), name };
        return node;
    }

    static auto create_label_def(const SourceInfo& info, std::string_view name_a, ArenaMemoryResource& arena) -> arena_ptr<ParserIR>
    {
        auto node = new (arena, alignof(ParserIR)) ParserIR;
        auto name = create_upper_view(name_a, arena);
        node->op = LabelDef { create_source_info(info, arena), name };
        return node;
    }
                            

    static auto create_integer(const SourceInfo& info, int32_t value_,
                              ArenaMemoryResource& arena) -> arena_ptr<Argument>
    {
        decltype(Argument::value) value = value_;
        arena_ptr<Argument> arg_ptr = new (arena, alignof(Argument)) Argument { create_source_info(info, arena), value };
        return arg_ptr;
    }

    static auto create_float(const SourceInfo& info, float value_,
                           ArenaMemoryResource& arena) -> arena_ptr<Argument>
    {
        auto arg = create_integer(info, 0, arena);
        arg->value = value_;
        return arg;
    }

    static auto create_identifier(const SourceInfo& info,
                                std::string_view name,
                                ArenaMemoryResource& arena) -> arena_ptr<Argument>
    {
        auto arg = create_integer(info, 0, arena);
        arg->value = Identifier { create_upper_view(name, arena) };
        return arg;
    }

    static auto create_string(const SourceInfo& info,
                            std::string_view string,
                            ArenaMemoryResource& arena) -> arena_ptr<Argument>
    {
        auto arg = create_integer(info, 0, arena);
        arg->value = String { create_string_view(string, arena) };
        return arg;
    }

    static auto create_upper_view(std::string_view from, ArenaMemoryResource& arena)
        -> std::string_view
    {
        auto ptr = (char*) arena.allocate(from.size(), alignof(char));
        std::transform(from.begin(), from.end(), ptr, [](unsigned char c) {
            return c >= 'a' && c <= 'z'? (c - 32) : c;
        });
        return std::string_view(ptr, from.size());
    }

    static auto create_string_view(std::string_view from, ArenaMemoryResource& arena)
        -> std::string_view
    {
        auto ptr = (char*) arena.allocate(from.size(), alignof(char));
        std::memcpy(ptr, from.data(), from.size());
        return std::string_view(ptr, from.size());
    }
};
// TODO static_assert trivial destructible everything

template<typename T>
struct LinkedIR
{
public:
    explicit LinkedIR() = default;

    static auto from_ir(arena_ptr<T> ir) -> LinkedIR
    {
        LinkedIR linked;
        linked.setup_first(ir);
        return linked;
    }

    LinkedIR(const LinkedIR&) = delete;

    LinkedIR(LinkedIR&& other) :
        front_(other.front_), back_(other.back_)
    {
        other.front_ = other.back_ = nullptr;
    }

    LinkedIR& operator=(const LinkedIR&) = delete;

    LinkedIR& operator=(LinkedIR&& other)
    {
        LinkedIR(std::move(other)).swap(*this);
        return *this;
    }

    void swap(LinkedIR& other)
    {
        std::swap(this->front_, other.front_);
        std::swap(this->back_, other.back_);
    }

    bool empty() const
    {
        return front_ == nullptr;
    }

    auto front() const -> arena_ptr<T>
    {
        assert(front_ != nullptr);
        return front_;
    }

    auto back() const -> arena_ptr<T>
    {
        assert(back_ != nullptr);
        return back_;
    }

    // O(1)
    void push_front(arena_ptr<T> ir)
    {
        assert(ir && !ir->prev && !ir->next);

        if(empty())
            return setup_first(ir);

        ir->set_next(this->front_);
        this->front_ = ir;
    }

    // O(1)
    void push_back(arena_ptr<T> ir)
    {
        assert(ir && !ir->prev && !ir->next);

        if(empty())
            return setup_first(ir);

        this->back_->set_next(ir);
        this->back_ = ir;
    }

    // O(1)
    void splice_back(LinkedIR&& other)
    {
        if(this->empty())
            return other.swap(*this);

        if(other.empty())
            return;

        this->back_->set_next(other.front_);
        this->back_ = other.back_;

        other.front_ = other.back_ = nullptr;
    }

private:
    static auto find_front(arena_ptr<T> ir) -> arena_ptr<T>
    {
        auto first = nullptr;
        while(ir)
        {
            first = ir;
            ir = ir->prev;
        }
        return first;
    }

    static auto find_back(arena_ptr<T> ir) -> arena_ptr<T>
    {
        auto last = nullptr;
        while(ir)
        {
            last = ir;
            ir = ir->next;
        }
        return last;
    }

    void setup_first(arena_ptr<T> ir)
    {
        assert(empty());

        this->front_ = ir;
        while(ir != nullptr)
        {
            this->back_ = ir;
            ir = ir->next;
        }
    }

private:
    arena_ptr<T> front_ = nullptr;
    arena_ptr<T> back_ = nullptr;
};


class Parser
{
public:
    explicit Parser(Scanner scanner_, ArenaMemoryResource& arena_) :
        scanner(std::move(scanner_)),
        arena(arena_)
    {
        assert(std::size(has_peek_token) == std::size(peek_tokens));
        std::fill(std::begin(has_peek_token), std::end(has_peek_token), false);
    }

    /// \returns the source file associated with this parser.
    auto source_file() const -> const SourceFile&;

    /// Skips to the next line in the token stream.
    void skip_current_line();

    auto parse_statement()
        -> std::optional<LinkedIR<ParserIR>>;

private:
    auto parse_embedded_statement()
        -> std::optional<LinkedIR<ParserIR>>;

    auto parse_scope_statement()
        -> std::optional<LinkedIR<ParserIR>>;

    auto parse_command_statement()
        -> std::optional<arena_ptr<ParserIR>>;

    auto parse_command(bool is_if_line = false) 
        -> std::optional<arena_ptr<ParserIR>>;

    auto parse_expression()
        -> std::optional<arena_ptr<ParserIR>>;

    auto parse_argument() 
        -> std::optional<arena_ptr<ParserIR::Argument>>;

    bool is_digit(char) const;
    bool is_integer(const Token&) const;
    bool is_float(const Token&) const;
    bool is_identifier(const Token&) const;

    auto source_info(const Token&) const -> ParserIR::SourceInfo;


private:
    /// Looks ahead by N tokens in the token stream.
    /// 
    /// In order to look into the token `N`, it is necessary to first look
    /// into the token `N-1` (unless`N == 0`). A call to a token consumer
    /// (such as `next` or `consume`) reduces the number of already peeked
    /// tokens by one.
    ///
    /// The lookahead is restricted to the line of the current token,
    /// thus trying to read past the end of the line yields `std::nullopt`.
    ///
    /// The 0th peek token is the current token in the stream.
    ///
    /// \returns the token at N or `std::nullopt` if such token is invalid.
    auto peek(size_t n = 0) -> std::optional<Token>;

    /// Consumes and returns the current token in the stream.
    ///
    /// \returns the current token or `std::nullopt` if such token is invalid.
    auto next() -> std::optional<Token>;

    /// Consumes and returns the current token in the stream.
    ///
    /// If the token category is none of `categories`, then `std::nullopt`
    /// is returned. Notice that is not the only case `std::nullopt` may
    /// get returned. Please see `next`.
    template<typename... Args>
    auto consume(Args... category0) -> std::optional<Token>
    {
        static_assert((std::is_same_v<Args, Category> && ...));

        if(sizeof...(Args) == 0)
            return next();

        auto token = next();
        if(!token)
            return std::nullopt;

        if(((token->category != category0) && ...))
        {
            // TODO emit expected error
            return std::nullopt;
        }

        return token;
    }

    auto consume_word(std::string_view lexeme) -> std::optional<Token>
    {
        auto token = consume(Category::Word);
        if(!token)
            return std::nullopt;
        if(token->lexeme != lexeme)
            return std::nullopt;
        return token;
    }

    bool is_peek(Category category, size_t n = 0)
    {
        return peek(n) && peek(n)->category == category;
    }

    bool is_peek(Category category, std::string_view lexeme, size_t n = 0)
    {
        return is_peek(category, n) && peek(n)->lexeme == lexeme;
    }

private:
    Scanner scanner;
    ArenaMemoryResource& arena;

    bool has_peek_token[6];
    std::optional<Token> peek_tokens[6];
    
    bool in_lexical_scope = false;

};
}
