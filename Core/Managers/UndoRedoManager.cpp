#include "UndoRedoManager.hpp"

#include <QImage>
#include <QPainter>

UndoRedoManager::UndoRedoManager() {}

void UndoRedoManager::pushUndo(const Delta& delta) {
    undoStack.push(delta);
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

bool UndoRedoManager::undo(QImage* image) {
    if (undoStack.empty()) return false;

    int bytesPerPixel = image->depth() / 8;

    Delta delta = undoStack.top();
    undoStack.pop();
    redoStack.push(Utilities::General::getImageCompressedDelta(image->bits(), image->width(), delta.region));

    auto size = delta.region.width() * delta.region.height() * bytesPerPixel;
    uint8_t* decompressed = new uint8_t[size];
    Utilities::Compression::decompressRunLengthEncoding(delta.data, decompressed, size);

    QPainter painter(image);
    painter.drawImage(delta.region, QImage(decompressed, delta.region.width(), delta.region.height(), image->format()));
            /* visualize */   painter.drawRect(delta.region.x(), delta.region.y(), delta.region.width() - 1, delta.region.height() - 1);
    delete[] decompressed;
    decompressed = nullptr;
    return true;
}

bool UndoRedoManager::redo(QImage* image) {
    if (redoStack.empty()) return false;

    int bytesPerPixel = image->depth() / 8;

    Delta delta = redoStack.top();
    redoStack.pop();

    undoStack.push(Utilities::General::getImageCompressedDelta(image->bits(), image->width(), delta.region));

    auto size = delta.region.width() * delta.region.height() * bytesPerPixel;
    uint8_t* decompressed = new uint8_t[size];
    Utilities::Compression::decompressRunLengthEncoding(delta.data, decompressed, size);

    QPainter painter(image);
    painter.drawImage(delta.region, QImage(decompressed, delta.region.width(), delta.region.height(), image->format()));
    delete[] decompressed;
    decompressed = nullptr;
    return true;
}
