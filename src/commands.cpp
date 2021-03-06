#include "commands.hpp"
#include "symtable.hpp"
#include "error.hpp"

static void match_identifier_var(const shared_ptr<Var>& var, const Command::Arg& arg, const SymTable& symbols)
{
    bool is_good = false;

    if((var->global && arg.allow_global_var) || (!var->global && arg.allow_local_var))
    {
        switch(var->type)
        {
            case VarType::Int:
                is_good = (arg.type == ArgType::Integer || arg.type == ArgType::Any);
                break;
            case VarType::Float:
                is_good = (arg.type == ArgType::Float || arg.type == ArgType::Any);
                break;
            case VarType::TextLabel:
                is_good = (arg.type == ArgType::TextLabel || arg.type == ArgType::Any);
                break;
            case VarType::TextLabel16:
                // TODO ArgType::TextLabel16?? (SA)
                is_good = (arg.type == ArgType::TextLabel || arg.type == ArgType::Any);
                break;
            default:
                Unreachable();
        }

        if(is_good == false)
            throw BadAlternator("XXX type mismatch");
    }
    else
        throw BadAlternator("XXX kind of var not allowed");
}

static void match_identifier(const SyntaxTree& node, const Commands& commands, const Command::Arg& arg, const SymTable& symbols, const shared_ptr<Scope>& scope_ptr)
{
    switch(arg.type)
    {
        case ArgType::Label:
            if(!symbols.find_label(node.text()))
                throw BadAlternator("XXX not label identifier");
            break;

        case ArgType::TextLabel:
            // Nothing to do, identifiers can be text labels with no checks.
            // TODO check for vars (SA)
            break;

        case ArgType::Integer:
        case ArgType::Float:
        case ArgType::Any:
        {
            if(arg.allow_constant && arg.type != ArgType::Float)
            {
                if(commands.find_constant_for_arg(node.text(), arg))
                    break;
            }

            if(auto opt_var = symbols.find_var(node.text(), scope_ptr))
            {
                match_identifier_var(*opt_var, arg, symbols);
                break;
            }

            throw BadAlternator("XXX");
        }

        default:
            Unreachable();
    }
}

template<typename Iter> static 
const Command& match_internal(const Commands& commands, const SymTable& symbols, const shared_ptr<Scope>& scope_ptr,
    Commands::alternator_pair alternator_range, Iter begin, Iter end)
{
    auto num_target_args = (size_t)std::distance(begin, end);

    for(auto it = alternator_range.first; it != alternator_range.second; ++it)
    {
        const Command& alternative = it->second;

        bool is_optional = false;
        size_t args_readen = 0;

        auto it_alter_arg = alternative.args.begin();
        auto it_target_arg = begin;

        for(; ;
        (it_alter_arg->optional? it_alter_arg : ++it_alter_arg),
            ++it_target_arg,
            ++args_readen)
        {
            assert(args_readen <= num_target_args);

            if(args_readen < num_target_args)
            {
                if(it_alter_arg == alternative.args.end())
                    break; // too many args on target
            }
            else // end of arguments
            {
                if(it_alter_arg == alternative.args.end() || it_alter_arg->optional)
                    return alternative;
                else
                    break; // too few args on target
            }

            bool bad_alternative = false;

            switch((*it_target_arg)->type())
            {
                case NodeType::Integer:
                    bad_alternative = !(argtype_matches(it_alter_arg->type, ArgType::Integer) && it_alter_arg->allow_constant);
                    break;
                case NodeType::Float:
                    bad_alternative = !(argtype_matches(it_alter_arg->type, ArgType::Float) && it_alter_arg->allow_constant);
                    break;
                case NodeType::Array:
                    // TODO array SA
                    break;
                case NodeType::Identifier:
                    try
                    {
                        match_identifier(**it_target_arg, commands, *it_alter_arg, symbols, scope_ptr);
                    }
                    catch(const CompilerError&)
                    {
                        bad_alternative = true;
                    }
                    break;
                case NodeType::ShortString:
                    bad_alternative = !(argtype_matches(it_alter_arg->type, ArgType::TextLabel) && it_alter_arg->allow_constant);
                case NodeType::LongString:
                    bad_alternative = !(argtype_matches(it_alter_arg->type, ArgType::TextLabel) || argtype_matches(it_alter_arg->type, ArgType::Buffer128));
                    bad_alternative = bad_alternative || !it_alter_arg->allow_constant;;
                    break;
            }

            if(bad_alternative)
                break; // try another alternative
        }
    }

    throw BadAlternator("XXX");
}

template<typename Iter> static
void annotate_internal(const Commands& commands, const SymTable& symbols, const shared_ptr<Scope>& scope_ptr,
    const Command& command, Iter begin, Iter end)
{
    size_t i = 0;

    // Expects all command_args (begin,end) matches command.args

    for(auto it = begin; it != end; ++it)
    {
        auto& arg = command.args[i];
        if(!arg.optional) ++i;

        SyntaxTree& arg_node = **it;

        switch((*it)->type())
        {
            case NodeType::Integer:
            {
                if(arg_node.is_annotated())
                    Expects(arg_node.maybe_annotation<const int32_t&>());
                else
                    arg_node.set_annotation(static_cast<int32_t>(std::stoi(arg_node.text(), nullptr, 0)));
                break;
            }
            case NodeType::Float:
            {
                if(arg_node.is_annotated())
                    Expects(arg_node.maybe_annotation<const float&>());
                else
                    arg_node.set_annotation(std::stof(arg_node.text()));
                break;
            }

            case NodeType::ShortString:
            case NodeType::LongString:
            {
                // TODO unescape and annotate SA
                break;
            }

            case NodeType::Array:
            {
                // TODO array SA
                break;
            }

            case NodeType::Identifier:
            {
                if(arg.type == ArgType::Label)
                {
                    if(arg_node.is_annotated())
                        Expects(arg_node.maybe_annotation<const shared_ptr<Label>&>());
                    else
                    {
                        shared_ptr<Label> label_ptr = symbols.find_label(arg_node.text()).value();
                        arg_node.set_annotation(label_ptr);
                    }
                }
                else if(arg.type == ArgType::TextLabel)
                {
                    if(arg_node.is_annotated())
                        Expects(arg_node.maybe_annotation<const std::string&>());
                    else
                        arg_node.set_annotation(arg_node.text());
                }
                else if(arg.type == ArgType::Integer || arg.type == ArgType::Float || arg.type == ArgType::Any)
                {
                    if(auto opt_const = commands.find_constant_for_arg(arg_node.text(), arg))
                    {
                        if(arg_node.is_annotated())
                            Expects(arg_node.maybe_annotation<const int32_t&>());
                        else
                            arg_node.set_annotation(*opt_const);
                    }
                    else if(auto opt_var = symbols.find_var(arg_node.text(), scope_ptr))
                    {
                        if(arg_node.is_annotated())
                            Expects(arg_node.maybe_annotation<const shared_ptr<Var>&>());
                        else
                            arg_node.set_annotation(*opt_var);
                    }
                    else
                        Unreachable();
                }
                else
                {
                    Unreachable();
                }
                break;
            }

            default:
                Unreachable();
        }
    }
}

////////

const Command& Commands::match(const SyntaxTree& command_node, const SymTable& symbols, const shared_ptr<Scope>& scope_ptr) const
{
    auto alternator_range = commands.equal_range(command_node.child(0).text());
    return ::match_internal(*this, symbols, scope_ptr, alternator_range, command_node.begin() + 1, command_node.end());
}

const Command& Commands::match_internal(const SymTable& symbols, const shared_ptr<Scope>& scope_ptr,
    alternator_pair alternator_range, const SyntaxTree** begin, const SyntaxTree** end) const
{
    return ::match_internal(*this, symbols, scope_ptr, alternator_range, begin, end);
}

void Commands::annotate(SyntaxTree& command_node, const Command& command, 
    const SymTable& symbols, const shared_ptr<Scope>& scope_ptr) const
{
    return ::annotate_internal(*this, symbols, scope_ptr, command, command_node.begin() + 1, command_node.end());
}

void Commands::annotate_internal(const SymTable& symbols, const shared_ptr<Scope>& scope_ptr,
    const Command& command, SyntaxTree** begin, SyntaxTree** end) const
{
    return ::annotate_internal(*this, symbols, scope_ptr, command, begin, end);
}

optional<int32_t> Commands::find_constant(const std::string& value, bool context_free_only) const
{
    if(context_free_only)
    {
        auto it = enums.find("");
        if(it != enums.end())
        {
            if(it->second != nullptr)
                return it->second->find(value);
        }
        return nullopt;
    }
    else
    {
        // TODO
        return nullopt;
    }
}

optional<int32_t> Commands::find_constant_for_arg(const std::string& value, const Command::Arg& arg) const
{
    if(auto opt_const = arg.find_constant(value))
        return opt_const;
    else if(auto opt_const = this->find_constant(value, true))
        return opt_const;
    return nullopt;
}
