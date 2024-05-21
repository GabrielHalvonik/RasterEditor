#pragma once

#include <stack>

#include "../Utilities/General.hpp"

class QImage;

using Delta = Utilities::General::Delta;

struct UndoRedoManager {

    UndoRedoManager();

    void pushUndo(const Delta&);
    bool undo(QImage*);
    bool redo(QImage*);

private:
    std::stack<Delta> undoStack { };
    std::stack<Delta> redoStack { };
};
