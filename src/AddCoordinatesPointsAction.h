#pragma once

#include <actions/ToggleAction.h>

class ImageLoaderPlugin;

using namespace mv::gui;

class AddCoordinatesPointsAction : public ToggleAction
{
public:
    AddCoordinatesPointsAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void setAddCoordinatesPointsSilently(bool addCoordinatesPoints);
    void updateRows();
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles = QList<int>());
    void updateStateFromModel();

private:
    ImageLoaderPlugin&  _imageLoaderPlugin;

};