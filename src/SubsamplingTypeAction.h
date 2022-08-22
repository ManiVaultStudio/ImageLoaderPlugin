#pragma once

#include <actions/OptionAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class SubsamplingTypeAction : public OptionAction
{
public:
    SubsamplingTypeAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void setCurrentIndexSilently(const std::int32_t& currentIndex);
    void updateRows();
    void updateStateFromModel();

private:
    ImageLoaderPlugin&      _imageLoaderPlugin;

    friend class Widget;
};