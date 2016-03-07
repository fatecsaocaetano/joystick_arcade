// DirectInput Joystick Wrapper
// Copyright 2001 by Jon Parise <jparise@cmu.edu>
//
// $Id: joystick.cpp,v 1.1.1.1 2001/10/29 19:47:56 jon Exp $

#include <stdio.h>
#include "joystick.h"

#define SAFE_RELEASE(p)     { if(p) { (p)->Release(); (p) = NULL; } }

Joystick::Joystick(unsigned int id)
{
    this->id = id;
    counter = 0;

    interface_ptr = NULL;
    joystick = NULL;
}

Joystick::~Joystick()
{
    close();
}


HRESULT Joystick::init()
{
    HRESULT hr;

    // Create a DirectInput device
    if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
                                       DIRECTINPUT_VERSION, 
                                       IID_IDirectInput8,
                                       (VOID**)&interface_ptr, NULL))) {
        return hr;
    }

    // Look for the first simple joystick we can find.
    if (FAILED(hr = interface_ptr->EnumDevices(DI8DEVCLASS_GAMECTRL, ::enumCallback,
                                    (LPVOID)this, DIEDFL_ATTACHEDONLY))) {
        return hr;
    }

    // Make sure we got a joystick
    if (joystick == NULL) {
        return E_FAIL;
    }

    // Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    if (FAILED(hr = joystick->SetDataFormat(&c_dfDIJoystick2))) {
        return hr;
    }

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if (FAILED(hr = joystick->SetCooperativeLevel(NULL, DISCL_EXCLUSIVE | DISCL_FOREGROUND))) {
        return hr;
    }

    return S_OK;
}

HRESULT Joystick::close()
{
    if (joystick) { 
        joystick->Unacquire();
    }
    
    SAFE_RELEASE(joystick);
    SAFE_RELEASE(interface_ptr);

    return S_OK;
}

HRESULT Joystick::poll(DIJOYSTATE2 *js)
{
    HRESULT hr;

    if (joystick == NULL) {
        return S_OK;
    }

    // Poll the device to read the current state
    hr = joystick->Poll(); 
    if (FAILED(hr)) {

        // DirectInput is telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so we
        // don't have any special reset that needs to be done.  We just
        // re-acquire and try again.
        hr = joystick->Acquire();
        while (hr == DIERR_INPUTLOST) {
            hr = joystick->Acquire();
        }

        // If we encounter a fatal error, return failure.
        if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED)) {
            return E_FAIL;
        }

        // If another application has control of this device, return success.
        // We'll just have to wait our turn to use the joystick.
        if (hr == DIERR_OTHERAPPHASPRIO) {
            return S_OK;
        }
    }

    // Get the input's device state
    if (FAILED(hr = joystick->GetDeviceState(sizeof(DIJOYSTATE2), js))) {
        return hr;
    }

    return S_OK;
}

bool Joystick::getButton(unsigned int num)
{
	if(joystick == NULL) return false;
	if(num >  128) return false;

	DIJOYSTATE2 js;
	this->poll(&js);
	return (js.rgbButtons[num] & 0x80);
}

float Joystick::getAxisX()
{
	if(this->joystick == NULL) return 0;
	
	DIJOYSTATE2 js;	
	this->poll(&js);
	return ((js.lX  / 32767.5) - 1);
}

float Joystick::getAxisY()
{
	if(this->joystick == NULL) return 0;
	
	DIJOYSTATE2 js;	
	this->poll(&js);
	return ((js.lY / 32767.5) - 1);
}

int Joystick::getAxisPosition ()
{
	int ret = 0;
	float x = 0;
	float y = 0;

	x = this->getAxisX();
	y = this->getAxisY();

	

	ret |= (x >  0.5) ? RIGHT:
	       (x < -0.5) ? LEFT :0;

	ret |= (y >  0.5) ? DOWN:
	       (y < -0.5) ? UP  :0;

	return ret;
}

BOOL CALLBACK Joystick::enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    // If this is the requested device ID ...
    if (counter == this->id) {

        // Obtain an interface to the enumerated joystick.  Stop the enumeration
        // if the requested device was created successfully.
        if (SUCCEEDED(interface_ptr->CreateDevice(instance->guidInstance, &joystick, NULL))) {
            return DIENUM_STOP;
        }  
    }

    // Otherwise, increment the device counter and continue with
    // the device enumeration.
    ++counter;

    return DIENUM_CONTINUE;
}

BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    if (context != NULL) {
        return ((Joystick *)context)->enumCallback(instance, context);
    } else {
        return DIENUM_STOP;
    }
}

unsigned int Joystick::deviceCount()
{
    unsigned int counter = 0;
    LPDIRECTINPUT8 interface_ptr = NULL;
    HRESULT hr;

    if (SUCCEEDED(hr = DirectInput8Create(GetModuleHandle(NULL),
                                          DIRECTINPUT_VERSION, 
                                          IID_IDirectInput8,
                                          (VOID**)&interface_ptr, NULL))) {
        interface_ptr->EnumDevices(DI8DEVCLASS_GAMECTRL, ::countCallback,
                        &counter, DIEDFL_ATTACHEDONLY);
    }

    return counter;
}

BOOL CALLBACK countCallback(const DIDEVICEINSTANCE* instance, VOID* counter)
{
    if (counter != NULL) {
        unsigned int *tmpCounter = (unsigned int *)counter;
        (*tmpCounter)++;
        counter = tmpCounter;
    }

    return DIENUM_CONTINUE;
}
