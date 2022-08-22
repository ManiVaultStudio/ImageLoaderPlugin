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
    void updateStateFromModel();

private:
    ImageLoaderPlugin&      _imageLoaderPlugin;

    friend class Widget;
};