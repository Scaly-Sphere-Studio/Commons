#pragma once

#include "_includes.hpp"

SSS_BEGIN;

// Ignore warning about STL exports as they're private members
#pragma warning(push, 2)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

class SSS_COMMONS_API CommandBase {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class SSS_COMMONS_API CommandHistory {
private:
    std::vector<std::unique_ptr<CommandBase>> _commands;
    size_t i = 0;
    bool _is_mergeable = false;
public:
    CommandHistory()                      = default;
    CommandHistory(CommandHistory const&) = delete;
    CommandHistory(CommandHistory&&)      = delete;

    void undo();
    void redo();

    template<std::derived_from<CommandBase> Command, class... T >
    void add(T... args) {
        bool const mergeable = _is_mergeable && i != 0;
        _commands.erase(_commands.cbegin() + i, _commands.cend());
        _commands.emplace_back(std::make_unique<Command>((args)...));
        redo();
        if (mergeable) {
            Command* last_cmd = dynamic_cast<Command*>(_commands.at(i - 2).get());
            Command* new_cmd = dynamic_cast<Command*>(_commands.at(i - 1).get());
            if (last_cmd && new_cmd && last_cmd->merge(*new_cmd)) {
                _commands.pop_back();
                --i;
            }
        }
        _is_mergeable = true;
    }
    void clear();
};

#pragma warning(pop)

SSS_END;