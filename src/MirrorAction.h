#pragma once

#include <actions/ToggleAction.h>
#include <actions/HorizontalGroupAction.h>

class ImageLoaderPlugin;

class MirrorAction : public mv::gui::HorizontalGroupAction
{
public:
    MirrorAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void setMirrorHorizontalSilently(bool mirrorHorizontal);
    void setMirrorVerticalSilently(bool mirrorVertical);

    void updateRows();
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles = QList<int>());
    void updateStateFromModel();

private:
    ImageLoaderPlugin&      _imageLoaderPlugin;
    mv::gui::ToggleAction   _horizontalAction;
    mv::gui::ToggleAction   _verticalAction;

};