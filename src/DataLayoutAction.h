#pragma once

#include <actions/OptionAction.h>

#include <QModelIndexList>

class ImageLoaderPlugin;

using namespace hdps::gui;

class DataLayoutAction : public OptionAction
{
public:
    DataLayoutAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void setCurrentIndexSilently(const std::int32_t& currentIndex);
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles = QList<int>());
    void updateRows();
    void updateStateFromModel();
    
private:
    ImageLoaderPlugin&      _imageLoaderPlugin;

    friend class Widget;
};