// TODO doc comments

#include "stdinc.h"
#include "disassembler.hpp"

struct DecompilerIR2;

template<typename T>
std::string decompile_data(const T&, DecompilerIR2&);
std::string decompile_data(const DecompiledData&, DecompilerIR2&);

struct DecompilerIR2
{
private:
    std::vector<DecompiledData> data;

protected:
    friend std::string decompile_data(const DecompiledCommand&, DecompilerIR2&);
    friend std::string decompile_data(const int8_t&, DecompilerIR2&);
    friend std::string decompile_data(const int16_t&, DecompilerIR2&);
    friend std::string decompile_data(const int32_t&, DecompilerIR2&);
    friend std::string decompile_data(const DecompiledLabelDef&, DecompilerIR2&);

    const Commands& commands;
    bool is_label_arg = false;

    const bool is_main_block = false;
    const std::string block_name;

    size_t base_offset;
    size_t script_size;

    std::map<size_t, size_t> label_ids; // <local_offset, id>

public:
    explicit DecompilerIR2(const Commands& commands, std::vector<DecompiledData> decompiled,
                           size_t base_offset, size_t script_size, std::string block_name, bool is_main_block)
        : commands(commands), data(std::move(decompiled)),
          block_name(std::move(block_name)), base_offset(base_offset), script_size(script_size),
        is_main_block(is_main_block)
    {
        size_t label_id = 0;
        size_t last_offset;

        for(auto& d : this->data)
        {
            if(is<DecompiledLabelDef>(d.data))
            {
                auto& label_def = get<DecompiledLabelDef>(d.data);
                
                if(label_id != 0)
                    Expects(label_def.offset > last_offset);

                Expects(label_def.offset >= this->base_offset
                    && label_def.offset < this->base_offset + this->script_size);

                this->label_ids.emplace(label_def.offset - this->base_offset, ++label_id);
                last_offset = label_def.offset;
            }
        }
    }

    template<typename OnDecompile>
    void decompile(OnDecompile callback)
    {
        for(auto& d : this->data)
        {
            callback(::decompile_data(d, *this));
        }
    }

    std::string decompile()
    {
        std::string output;
        this->decompile([&output](const std::string& line) {
            output += line;
            output += '\n';
        });
        return output;
    }

    optional<std::string> decompile_label_arg(int value)
    {
        auto make_output = [&](char c, const std::string& block_name, size_t offset) -> optional<std::string>
        {
            if(offset >= this->base_offset
                && offset < this->base_offset + this->script_size)
            {
                auto it = this->label_ids.find(offset - this->base_offset);
                if(it != this->label_ids.end())
                {
                    std::string output;
                    output.push_back(c);
                    output += block_name;
                    output.push_back('_');
                    output += std::to_string(it->second);
                    return output;
                }
            }
            return nullopt;
        };

        if(value >= 0)
        {
            if(this->is_main_block)
                return make_output('@', block_name, value);
            else
                return nullopt;
        }
        else
            return make_output('%', block_name, this->base_offset + size_t(-value));
    }
};


template<typename T>
inline std::string decompile_data(const T& x, DecompilerIR2&)
{
    return x.decompile_data();
}

inline std::string decompile_data(const EOAL&, DecompilerIR2&)
{
    return std::string();
}

inline std::string decompile_data(const int8_t& value, DecompilerIR2& context)
{
    if(context.is_label_arg)
    {
        if(auto opt_output = context.decompile_label_arg(value))
            return *opt_output;
    }

    std::string output = std::to_string(static_cast<int>(value));
    output += "i8";
    return output;
}

inline std::string decompile_data(const int16_t& value, DecompilerIR2& context)
{
    if(context.is_label_arg)
    {
        if(auto opt_output = context.decompile_label_arg(value))
            return *opt_output;
    }

    std::string output = std::to_string(value);
    output += "i16";
    return output;
}

inline std::string decompile_data(const int32_t& value, DecompilerIR2& context)
{
    if(context.is_label_arg)
    {
        if(auto opt_output = context.decompile_label_arg(value))
            return *opt_output;
    }

    std::string output = std::to_string(value);
    output += "i32";
    return output;
}

inline std::string decompile_data(const float& value, DecompilerIR2&)
{
    std::string output = std::to_string(value);
    output.erase(output.find_last_not_of('0') + 1, std::string::npos);
    if(output.back() == '.') output.push_back('0');
    output += "f";
    return output;
}

inline std::string decompile_data(const DecompiledString& str, DecompilerIR2&)
{
    std::string output;
    char quotes = 0;

    output.reserve(str.storage.size() + 3); // +3 for quotes

    switch(str.type)
    {
        case CompiledString::Type::TextLabel8:
            output += "'";
            quotes = '\'';
            break;
        case CompiledString::Type::TextLabel16:
            output += "v'";
            quotes = '\'';
            break;
        case CompiledString::Type::StringVar:
            output += "\"";
            quotes = '"';
            break;
        case CompiledString::Type::String128:
            output += "b\"";
            quotes = '"';
            break;
        default:
            Unreachable();
    }

    // TODO parse to escape sequences
    for(size_t i = 0; i < str.storage.size(); ++i)
    {
        if(str.storage[i] == '\0') break;
        output.push_back(str.storage[i]);
    }

    output.push_back(quotes);

    return output;
}

inline std::string decompile_data(const DecompiledVar& v, DecompilerIR2&)
{
    std::string output;
    if(v.global)
    {
        output.push_back('&');
        output.append(std::to_string(v.offset));
    }
    else
    {
        output.append(std::to_string(v.offset / 4));
        output.push_back('@');
    }
    return output;
}

inline std::string decompile_data(const DecompiledVarArray& v, DecompilerIR2& context)
{
    std::string output;
    output += decompile_data(v.base, context);
    output.push_back('(');
    output += decompile_data(v.index, context);
    output.push_back(',');
    output += std::to_string(static_cast<int>(v.array_size));
    output += v.elem_type == DecompiledVarArray::ElementType::None? "" :
              v.elem_type == DecompiledVarArray::ElementType::Int? "i" :
              v.elem_type == DecompiledVarArray::ElementType::Float? "f" :
              v.elem_type == DecompiledVarArray::ElementType::TextLabel? "s" :
              v.elem_type == DecompiledVarArray::ElementType::TextLabel16? "v" :
              Unreachable();
    output.push_back(')');
    return output;
}

inline std::string decompile_data(const ArgVariant2& varg, DecompilerIR2& context)
{
    return visit_one(varg, [&](const auto& arg) { return ::decompile_data(arg, context); });
}

inline std::string decompile_data(const DecompiledCommand& ccmd, DecompilerIR2& context)
{
    std::string output;
    size_t arg_id = 0;
    
    optional<const Command&> opt_command = context.commands.find_command(ccmd.id & 0x7FFF);

    bool not_flag = (ccmd.id & 0x8000) != 0;
    auto cmd_name = *context.commands.find_command_name(ccmd.id & 0x7FFF, true);
    
    output.reserve(12 + cmd_name.size() + ccmd.args.size() * 6);

    /*
    char opcode_buffer[6+1];
    snprintf(opcode_buffer, sizeof(opcode_buffer), "%.4X: ", ccmd.id);
    output.append(std::begin(opcode_buffer), std::end(opcode_buffer) - 1);
    */

    if(not_flag) output += "NOT ";
    output += cmd_name;
    output.push_back(' ');
    for(size_t i = 0; i < ccmd.args.size(); ++i)
    {
        if(opt_command)
        {
            if(auto opt_arg = opt_command->arg(i))
                context.is_label_arg = (opt_arg->type == ArgType::Label);
        }

        output += ::decompile_data(ccmd.args[i], context);
        output.push_back(' ');

        context.is_label_arg = false;
    }

    return output;
}

inline std::string decompile_data(const DecompiledLabelDef& label, DecompilerIR2& context)
{
    return fmt::format("{}_{}:", context.block_name, context.label_ids[label.offset - context.base_offset]);
}

inline std::string decompile_data(const DecompiledHex& hex, DecompilerIR2&)
{
    std::string output;
    output.reserve(sizeof("\nHEX\n") + (hex.data.size() * 3) + sizeof("\nENDHEX\n\n") + 32);

    output += "HEX\n";
    for(auto& x : hex.data)
    {
        char buffer[3+1];
        snprintf(buffer, sizeof(buffer), "%.2X ", x);
        output.append(std::begin(buffer), std::end(buffer) - 1);
    }
    output += "\nENDHEX";
    return output;
}

inline std::string decompile_data(const DecompiledData& data, DecompilerIR2& context)
{
    return visit_one(data.data, [&](const auto& data) { return ::decompile_data(data, context); });
}

