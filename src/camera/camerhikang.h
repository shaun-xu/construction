#ifndef CAMERHIKANG_H
#define CAMERHIKANG_H
#include "MvCameraControl.h"
#include "Camera.h"
#include <vector>


class CamerHikang : public Camera
{
public:
    // Static methods
    static std::vector<CameraInfo> getCameraList();
    // Interface function
    CamerHikang(unsigned int camNum, CameraTriggerMode triggerMode);
    CameraSettings getCameraSettings();
    void setCameraSettings(CameraSettings);
    void startCapture();
    void stopCapture();
    CameraFrame getFrame();
    size_t getFrameSizeBytes();
    size_t getFrameWidth();
    size_t getFrameHeight();
    ~CamerHikang();

private:
    void    *m_pHandle; //camer handle
    static MV_CC_DEVICE_INFO_LIST  m_stDevList;
    MV_CC_DEVICE_INFO  m_stDevInfo;
    int     m_nWidth;
    int     m_nHeight;
    bool  capturing;
};

#endif // CAMERHIKANG_H
