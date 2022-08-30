#pragma once

#include <actions/PluginTriggerPickerAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class ConversionAction : public PluginTriggerPickerAction
{
public:
    ConversionAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void setCurrentPluginTriggerActionSilently(const QString& currentPluginTriggerSha);
    void updateRows();
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles = QList<int>());
    void updateStateFromModel();

private:
    ImageLoaderPlugin&      _imageLoaderPlugin;

    friend class Widget;
};