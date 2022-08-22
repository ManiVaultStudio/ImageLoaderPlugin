#pragma once

#include <actions/WidgetAction.h>
#include <actions/IntegralAction.h>
#include <actions/OptionAction.h>
#include <actions/StringAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class NumberOfLevelsActions : public WidgetAction
{
public:

    /** Subsampling level factors */
    enum LevelFactor {
        Level2,     /** Scale each level by factor 2 */
        Level4,     /** Scale each level by factor 4 */
        Level8,     /** Scale each level by factor 8 */
        Level16     /** Scale each level by factor 16 */
    };

    static const QMap<LevelFactor, std::uint32_t> levelFactors;
    static const QMap<LevelFactor, QString> levelFactorNames;

protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, NumberOfLevelsActions* numberOfLevelsActions, const std::int32_t& widgetFlags);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    NumberOfLevelsActions(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void updateStateFromModel();

private:
    ImageLoaderPlugin&      _imageLoaderPlugin;         /** Reference to parent image loader plugin */
    IntegralAction          _numberOfLevelsAction;      /** Subsampling number of levels action */
    OptionAction            _levelFactorAction;              /** Subsampling levels factor action */
    StringAction            _infoAction;                /** Subsampling info action */

    friend class Widget;
};