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
    void updateRows();

protected:
    ImageLoaderPlugin&      _imageLoaderPlugin;
    QModelIndexList         _indices;

    friend class Widget;
};