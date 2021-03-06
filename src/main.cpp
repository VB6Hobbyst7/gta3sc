#include "stdinc.h"
#include "parser.hpp"
#include "error.hpp"
#include "symtable.hpp"
#include "commands.hpp"
#include "compiler.hpp"
#include "disassembler.hpp"
#include "decompiler.hpp"
#include "codegen.hpp"
#include "defs/defs.hpp"

extern void convert();

int test_compiler(const GameConfig& gta3_config, const Commands& commands);
int test_decompiler(const GameConfig& gta3_config, const Commands& commands);
int test_dasc0(const GameConfig& gta3_config, const Commands& commands);

int main()
{
    _wchdir(L"../..");

    GameConfig gta3_config;
    gta3_config.has_text_label_prefix = false;
    gta3_config.use_half_float = true;

    Commands commands = gta3_commands();

    return test_dasc0(gta3_config, commands);
}

int test_compiler(const GameConfig& gta3_config, const Commands& commands)
{
    std::vector<shared_ptr<Script>> scripts;

    auto main = Script::create("test.sc", ScriptType::Main);
    auto symbols = SymTable::from_script(*main);
    symbols.apply_offset_to_vars(2);

    scripts.emplace_back(main);

    auto subdir = main->scan_subdir();

    auto ext_scripts = read_and_scan_symbols(subdir, symbols.extfiles.begin(), symbols.extfiles.end(), ScriptType::MainExtension);
    auto sub_scripts = read_and_scan_symbols(subdir, symbols.subscript.begin(), symbols.subscript.end(), ScriptType::Subscript);
    auto mission_scripts = read_and_scan_symbols(subdir, symbols.mission.begin(), symbols.mission.end(), ScriptType::Mission);

    for(auto& x : ext_scripts)
    {
        symbols.merge(std::move(x.second));
        scripts.emplace_back(x.first); // maybe move
    }

    for(auto& x : sub_scripts)
    {
        symbols.merge(std::move(x.second));
        scripts.emplace_back(x.first); // maybe move
    }

    for(auto& x : mission_scripts)
    {
        symbols.merge(std::move(x.second));
        scripts.emplace_back(x.first); // maybe move
    }

    symbols.build_script_table(scripts);

    for(auto& script : scripts)
    {
        script->annotate_tree(symbols, commands);
    }


    std::vector<CodeGenerator> gens;
    for(auto& script : scripts)
    {
        CompilerContext cc(script, symbols, commands);
        cc.compile();
        gens.emplace_back(gta3_config, std::move(cc));
    }

    // not thread-safe
    Expects(gens.size() == scripts.size());
    for(size_t i = 0; i < gens.size(); ++i) // zip
    {
        scripts[i]->size = gens[i].compute_labels(); // <- maybe???! this is actually thread safe
    }                                                //
    Script::compute_script_offsets(scripts);         // <- but this isn't


    for(auto& gen : gens)
        gen.generate();

    if(FILE* f = fopen("output.cs", "wb"))
    {
        auto guard = make_scope_guard([&] {
            fclose(f);
        });

        for(auto& gen : gens)
        {
            write_file(f, gen.bytecode.get(), gen.script->size.value());
        }
    }
    else
    {
        throw CompilerError("XXX");
    }

    return 0;
}

int test_decompiler(const GameConfig& gta3_config, const Commands& commands)
{
    auto decomp = Disassembler::from_file(gta3_config, commands, "output.cs");

    decomp.run_analyzer();
    auto data = decomp.get_data();

    printf("%d\n", data.size());

    puts(DecompilerContext(commands, std::move(data)).decompile().c_str());

    return 0;
}

extern int test_dasc(const Commands& commands, std::vector<DecompiledData> decompiled);

int test_dasc0(const GameConfig& gta3_config, const Commands& commands)
{
    auto decomp = Disassembler::from_file(gta3_config, commands, "output.cs");

    decomp.run_analyzer();

    test_dasc(commands, decomp.get_data());

    return 0;
}
