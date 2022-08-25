#pragma once

#include <actions/StringAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class DatasetNameAction : public StringAction
{
public:
    DatasetNameAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void setDatasetNameSilently(const QString& datasetName);
    void updateRows();
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles = QList<int>());
    void updateStateFromModel();

private:
    ImageLoaderPlugin&      _imageLoaderPlugin;

    friend class Widget;
};