#pragma once
#include <cstdint>
#include <gta3sc/command-manager.hpp>
#include <gta3sc/sourceman.hpp>
#include <gta3sc/util/arena-allocator.hpp>
#include <optional>
#include <unordered_map>
#include <vector>

// TODO review the constness of stuff here

namespace gta3sc
{
class SymbolRepository;

/// Uniquely identifiers a scope.
using ScopeId = uint32_t;

/// Represents a defined label.
struct SymLabel
{
    SourceRange source; ///< The location the label was declared.
    ScopeId scope{};    ///< The scope on which the label is in.
};

/// Represents a declared variable.
struct SymVariable
{
    enum class Type : uint8_t
    {
        INT,
        FLOAT,
        TEXT_LABEL,
    };

    SourceRange source; ///< The location the variable was declared.
    uint32_t id{};      ///< The ordering of this variable in its scope.
    ScopeId scope{};    ///< The scope on which the variable is in.
    Type type{};        ///< The type of the variable.
    std::optional<uint16_t> dim; ///< Array dimensions if any.

    [[nodiscard]] auto is_array() const -> bool { return !!dim; }
};

/// Represents an used object.
struct SymUsedObject
{
    SourceRange
            source; ///< The location the object was used for the first time.
    uint32_t id{};  ///< The ordering of this used object (0-indexed).
};

/// An unordered associative container mapping names to symbols of type `T`.
template<typename T>
class SymbolTable : private std::unordered_map<std::string_view, arena_ptr<T>>
{
private:
    using super_type = std::unordered_map<std::string_view, arena_ptr<T>>;

protected:
    // We inherit (privately!) from unordered_map and exclude the
    // container modification methods from the public interface.
    //
    // As such, only SymbolRepository is able to modify the container.
    friend class SymbolRepository;

    using super_type::emplace;

public:
    using super_type::begin;
    using super_type::cbegin;
    using super_type::cend;
    using super_type::count;
    using super_type::empty;
    using super_type::end;
    using super_type::find;
    using super_type::size;
    using typename super_type::const_iterator;
    using typename super_type::iterator;
};

/// This is repository of symbols.
///
/// It shelters multiple symbol tables, each of which does not collide with
/// the symbols in other tables.
///
/// The repository contains the following tables:
///
///  + A symbol table of labels.
///  + A symbol table for global variables (at `scope_id = 0`).
///  + A symbol table for local variables in each scope.
///
class SymbolRepository
{
private:
    ArenaMemoryResource* arena;

public:
    /// The symbol table of labels.
    SymbolTable<const SymLabel> label_table; // NOLINT: FIXME

    /// The symbol table for used objects.
    SymbolTable<const SymUsedObject> used_objects; // NOLINT: FIXME

    /// The symbol table for each scope.
    ///
    /// The table at the i-th index of the vector stores the symbol table
    /// for the scope identified by the number `i`.
    ///
    /// The scope of index `0` is used for storing global variables and
    /// is always present in the repository.
    std::vector<SymbolTable<const SymVariable>> var_tables; // NOLINT: FIXME

public:
    explicit SymbolRepository(ArenaMemoryResource& arena);

    SymbolRepository(const SymbolRepository&) = delete;
    auto operator=(const SymbolRepository&) -> SymbolRepository& = delete;

    SymbolRepository(SymbolRepository&&) noexcept = default;
    auto operator=(SymbolRepository&&) noexcept -> SymbolRepository& = default;

    ~SymbolRepository() noexcept = default;

    /// Allocates a scope and returns its identifier.
    ///
    /// This effectively creates a symbol table for the scope.
    ///
    /// It is guaranted that successive calls to this function provides
    /// an integer value that is the successor of the previous returned value.
    auto allocate_scope() -> ScopeId;

    /// Lookups a variable in a certain scope.
    ///
    /// Do note looking up in a local variable scope does not
    /// automatically looks up in the global scope.
    ///
    /// \return the variable or `nullptr` if it does not exist.
    auto lookup_var(std::string_view name, ScopeId scope_id = 0) const
            -> const SymVariable*;

    /// Lookups a label.
    ///
    /// \return the label or `nullptr` if it does not exist.
    auto lookup_label(std::string_view name) const -> const SymLabel*;

    /// Lookups a used object.
    ///
    /// \return the used object or `nullptr` if never used.
    auto lookup_used_object(std::string_view name) const
            -> const SymUsedObject*;

    /// Inserts a new variable into a certain scope.
    ///
    /// No insertion takes place if the variable already exists in the
    /// specified scope.
    ///
    /// \param name the name of the variable.
    /// \param scope_id the scope to insert the variable into.
    /// \param type the typing of the variable.
    /// \param dim the array dimensions of the variable if any.
    /// \param source the location of its declaration in the source code.
    ///
    /// \return a pair with the variable and a boolean indicating whether
    ///         any insertion took place.
    auto insert_var(std::string_view name, ScopeId scope_id,
                    SymVariable::Type type, std::optional<uint16_t> dim,
                    SourceRange source) -> std::pair<const SymVariable*, bool>;

    /// Inserts a label into the symbol table.
    ///
    /// No insertion takes place if a label of same name already exists.
    ///
    /// \param name the name of the label.
    /// \param source the location of its declaration.
    /// \param scope_id the scope on which the label is in.
    ///
    /// \return a pair with the label and a boolean indicating whether
    ///         any insertion took place.
    auto insert_label(std::string_view name, ScopeId scope_id,
                      SourceRange source) -> std::pair<const SymLabel*, bool>;

    /// Inserts an used object into the symbol table.
    ///
    /// No insertion takes place if an used object of same name already exists.
    ///
    /// \param name the model name of the used object.
    /// \param source the location the used object was seen.
    ///
    /// \return a pair with the used object and a boolean indicating whether
    ///         any insertion took place.
    auto insert_used_object(std::string_view name, SourceRange source)
            -> std::pair<const SymUsedObject*, bool>;
};
} // namespace gta3sc
