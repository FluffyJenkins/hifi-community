//
//  TouchscreenVirtualPadDevice.h
//  input-plugins/src/input-plugins
//
//  Created by Triplelexx on 1/31/16.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_TouchscreenVirtualPadDevice_h
#define hifi_TouchscreenVirtualPadDevice_h

#include <controllers/InputDevice.h>
#include "InputPlugin.h"
#include <QtGui/qtouchdevice.h>
#include "VirtualPadManager.h"

class QTouchEvent;
class QGestureEvent;

const float STICK_RADIUS_INCHES = .3f;

class TouchscreenVirtualPadDevice : public InputPlugin {
Q_OBJECT
public:

    // Plugin functions
    virtual void init() override;
    virtual bool isSupported() const override;
    virtual const QString getName() const override { return NAME; }

    bool isHandController() const override { return false; }

    virtual void pluginFocusOutEvent() override { _inputDevice->focusOutEvent(); }
    virtual void pluginUpdate(float deltaTime, const controller::InputCalibrationData& inputCalibrationData) override;

    void touchBeginEvent(const QTouchEvent* event);
    void touchEndEvent(const QTouchEvent* event);
    void touchUpdateEvent(const QTouchEvent* event);
    void touchGestureEvent(const QGestureEvent* event);

    static const char* NAME;

protected:

    class InputDevice : public controller::InputDevice {
    public:
        InputDevice() : controller::InputDevice("TouchscreenVirtualPad") {}
    private:
        // Device functions
        virtual controller::Input::NamedVector getAvailableInputs() const override;
        virtual QString getDefaultMappingConfig() const override;
        virtual void update(float deltaTime, const controller::InputCalibrationData& inputCalibrationData) override;
        virtual void focusOutEvent() override;

        friend class TouchscreenVirtualPadDevice;
    };

public:
    const std::shared_ptr<InputDevice>& getInputDevice() const { return _inputDevice; }

protected:
    float _lastPinchScale;
    float _pinchScale;
    float _screenDPI;
    qreal _screenDPIProvided;
    glm::vec2 _screenDPIScale;
    bool _validTouchLeft;
    glm::vec2 _firstTouchLeftPoint;
    glm::vec2 _currentTouchLeftPoint;
    bool _validTouchRight;
    glm::vec2 _firstTouchRightPoint;
    glm::vec2 _currentTouchRightPoint;
    int _touchPointCount;
    int _screenWidthCenter;
    std::shared_ptr<InputDevice> _inputDevice { std::make_shared<InputDevice>() };

    bool _fixedPosition;
    glm::vec2 _fixedCenterPosition;
    float _fixedRadius;
    float _fixedRadiusForCalc;
    int _extraBottomMargin {0};

    void touchLeftBegin(glm::vec2 touchPoint);
    void touchLeftUpdate(glm::vec2 touchPoint);
    void touchLeftEnd();
    bool touchLeftBeginPointIsValid(glm::vec2 touchPoint);
    void touchRightBegin(glm::vec2 touchPoint);
    void touchRightUpdate(glm::vec2 touchPoint);
    void touchRightEnd();
    void setupFixedCenter(VirtualPad::Manager& virtualPadManager, bool force = false);

    void processInputUseCircleMethod(VirtualPad::Manager& virtualPadManager);
    void processInputUseSquareMethod(VirtualPad::Manager& virtualPadManager);
    glm::vec2 clippedPointInCircle(float radius, glm::vec2 origin, glm::vec2 touchPoint);
// just for debug
private:
    void debugPoints(const QTouchEvent* event, QString who);

};

#endif // hifi_TouchscreenVirtualPadDevice_h