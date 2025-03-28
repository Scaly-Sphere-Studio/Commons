#include "Commons/Command.hpp"

SSS_BEGIN;

void CommandHistory::undo()
{
    if (i != 0) {
        _commands.at(--i)->undo();
        _is_mergeable = false;
    }
}

void CommandHistory::redo()
{
    if (i != _commands.size()) {
        _commands.at(i++)->execute();
        _is_mergeable = false;
    }
}

void CommandHistory::clear()
{
    _commands.clear();
    i = 0;
}

SSS_END;