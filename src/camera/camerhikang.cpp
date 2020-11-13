#include "camerhikang.h"

#include "CameraIIDC.h"
#include <cstring>
#include <unistd.h>
#include <opencv2/opencv.hpp>

MV_CC_DEVICE_INFO_LIST  CamerHikang::m_stDevList;

vector<CameraInfo> CamerHikang::getCameraList(){

     vector<CameraInfo> ret;

     int nRet = -1;

     //枚举子网内指定的传输协议对应的所有设备
      unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
    // MV_CC_DEVICE_INFO_LIST m_stDevList = {0};
      nRet = MV_CC_EnumDevices(nTLayerType, &m_stDevList);
     if (MV_OK != nRet)
     {
         printf("error: EnumDevices fail [%x]\n", nRet);
         return ret;
     }

     int i = 0;
     if (m_stDevList.nDeviceNum == 0)
     {
         printf("no camera found!\n");
         return ret;
     }
     for(unsigned  int i=0;  i< m_stDevList.nDeviceNum; i++)
     {
        CameraInfo  info;
        info.vendor="hiking";
        info.model="hik_model";
        info.busID=i;
        ret.push_back(info);
     }


    return ret;
}

CamerHikang::CamerHikang(unsigned int camNum, CameraTriggerMode triggerMode) : Camera(triggerMode) {
    return;
    int ret = -1;
    m_pHandle = NULL;

    //枚举子网内指定的传输协议对应的所有设备
     unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
    int nRet = MV_CC_EnumDevices(nTLayerType, &m_stDevList);
    if (MV_OK != nRet)
    {
        printf("error: EnumDevices fail [%x]\n", nRet);
        return;
    }

    int i = 0;
    if (m_stDevList.nDeviceNum == 0)
    {
        printf("no camera found!\n");
        return;
    }

    //选择查找到的第一台在线设备，创建设备句柄
    int nDeviceIndex = 0;

    MV_CC_DEVICE_INFO m_stDevInfo = {0};
    memcpy(&m_stDevInfo, m_stDevList.pDeviceInfo[nDeviceIndex], sizeof(MV_CC_DEVICE_INFO));



     nRet = MV_CC_CreateHandle(&m_pHandle, &m_stDevInfo);

    if (MV_OK != nRet)
    {
        printf("error: CreateHandle fail [%x]\n", nRet);
        return;
    }

    //连接设备
     nRet = MV_CC_OpenDevice(m_pHandle,MV_ACCESS_Monitor);
    if (MV_OK != nRet)
    {
        printf("error: OpenDevice fail [%x]\n", nRet);
        return;
    }

//    //设置Enum型参数
//    unsigned int enMode = MV_TRIGGER_SOURCE_SOFTWARE; //设置触发源为软触发
//    nRet = MV_CC_SetTriggerSource(m_pHandle, enMode);
//   if (MV_OK != nRet)
//   {
//       printf("error: SetTriggerSource fail [%x]\n", nRet);
//       return;
//   }


    // Set reasonable default settings
    CameraSettings settings;
    //settings.shutter = 8.333;
    settings.shutter = 16.66;
    settings.gain = 0.0;
    this->setCameraSettings(settings);

    return;
}



CameraSettings CamerHikang::getCameraSettings(){

    // Get settings:
    CameraSettings settings;
    settings.gain = 16.66;//getSetting(cam, DC1394_FEATURE_GAIN);
    settings.shutter = 0.0;//getSetting(cam, DC1394_FEATURE_SHUTTER);

    return settings;
}

//void setSetting(dc1394camera_t *cam, dc1394feature_t setting, float value){
//    dc1394error_t err;
//    float min, max;
//    dc1394_feature_get_absolute_boundaries(cam, setting, &min, &max);
//    if(value < min || value > max){
//        std::cerr << "CameraIIDC: cannot set value. Out of permissable range." << std::endl;
//    } else {
//        err = dc1394_feature_set_absolute_control(cam, setting, DC1394_ON);
//        if(err != DC1394_SUCCESS)
//            cerr << "Could not enable absolute control!" << endl;
//        err = dc1394_feature_set_absolute_value(cam, setting, value);
//        if(err != DC1394_SUCCESS)
//            cerr << "Could not set absolute value!" << endl;
//    }
//}

void CamerHikang::setCameraSettings(CameraSettings settings){

}


void CamerHikang::startCapture(){
    capturing=true;
}

void CamerHikang::stopCapture(){
    capturing=false;
    //停止采集图像
    int nRet = MV_CC_StopGrabbing(m_pHandle);
    if (MV_OK != nRet)
    {
        printf("error: StopGrabbing fail [%x]\n", nRet);
        return;
    }

}

void __stdcall ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
    if (pFrameInfo)
    {
        // 输出时加上当前系统时间
         char   szInfo[128] = {0};
//        SYSTEMTIME sys;
//        GetLocalTime( &sys );
//        sprintf_s(szInfo, 128, "[%d-%02d-%02d %02d:%02d:%02d:%04d] : GetOneFrame succeed, width[%d], height[%d]", sys.wYear, sys.wMonth,
//                  sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, pFrameInfo->nWidth, pFrameInfo->nHeight);

        printf("%s\n", szInfo);
    }
}


CameraFrame  CamerHikang::getFrame()
{
    MV_CC_DEVICE_INFO_LIST stDeviceList;
    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    void* handle = NULL;

    CameraFrame  frame;
    // 枚举设备
    // enum device
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
    if (MV_OK != nRet)
    {

        std::cout<<"MV_CC_EnumDevices fail! nRet [%x]" <<nRet<<std::endl;
    }

    if (stDeviceList.nDeviceNum > 0)
    {
        for (int i = 0; i < stDeviceList.nDeviceNum; i++)
        {
            std::cout<<"[device %d]:" <<i<<std::endl;
            MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
            if (NULL == pDeviceInfo)
            {
                return frame;
            }
           // PrintDeviceInfo(pDeviceInfo);
        }
    }
    else
    {
        std::cout<<"Find No Devices!"<<std::endl;
        return frame;
    }

//    printf("Please Intput camera index: ");
    unsigned int nIndex = 0;
//    scanf("%d", &nIndex);

    if (nIndex >= stDeviceList.nDeviceNum)
    {
        std::cout<<"Intput error!"<<std::endl;
        return frame;
    }

    // 选择设备并创建句柄
    // select device and create handle
    nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
    if (MV_OK != nRet)
    {
        std::cout<<"MV_CC_CreateHandle fail! nRet [%x]"<<nRet<<std::endl;
        return frame;
    }

    // 打开设备
    // open device
    nRet = MV_CC_OpenDevice(handle);
    if (MV_OK != nRet)
    {
       std::cout<<"MV_CC_OpenDevice fail! nRet [%x]"<< nRet<<std::endl;
        return frame;
    }

    // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
    if (stDeviceList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
    {
        int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
        if (nPacketSize > 0)
        {
            nRet = MV_CC_SetIntValue(handle,"GevSCPSPacketSize",nPacketSize);
            if(nRet != MV_OK)
            {
                std::cout<<"Warning: Set Packet Size fail nRet [0x%x]!"<<nRet<<std::endl;
            }
        }
        else
        {
            std::cout<<"Warning: Get Packet Size fail nRet [0x%x]!"<<nPacketSize<<std::endl;
        }
    }

    // 设置触发模式为off
    // set trigger mode as off
    nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
    if (MV_OK != nRet)
    {
        std::cout<<"MV_CC_SetTriggerMode fail! nRet [%x]"<<nRet<<std::endl;
        return frame;
    }

    // 开始取流
    // start grab image
    nRet = MV_CC_StartGrabbing(handle);
    if (MV_OK != nRet)
    {
        std::cout<<"MV_CC_StartGrabbing fail! nRet [%x]"<<nRet<<std::endl;
        return frame;
    }

    // ch:获取数据包大小 | en:Get payload size
    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
    if (MV_OK != nRet)
    {
        std::cout<<"Get PayloadSize fail! nRet [0x%x]"<< nRet<<std::endl;
        return frame ;
    }

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    unsigned char * pData = (unsigned char *)malloc(sizeof(unsigned char) * stParam.nCurValue);
    if (NULL == pData)
    {
        return frame;
    }
    unsigned int nDataSize = stParam.nCurValue;

    int i=0;
    while(i++==0)
    {
        nRet = MV_CC_GetOneFrameTimeout(handle, pData, nDataSize, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            std::cout<<"GetOneFrame, Width[%d], Height[%d], nFrameNum[%d]"<<stImageInfo.nWidth<< stImageInfo.nHeight<<stImageInfo.nFrameNum<<std::endl;


            unsigned char  *pDataForRGB = (unsigned char*)malloc(stImageInfo.nWidth * stImageInfo.nHeight * 4 + 2048);
            if (NULL == pDataForRGB)
            {
                break;
            }
            // 像素格式转换
            // convert pixel format
            MV_CC_PIXEL_CONVERT_PARAM stConvertParam = {0};
            // 从上到下依次是：图像宽，图像高，输入数据缓存，输入数据大小，源像素格式，
            // 目标像素格式，输出数据缓存，提供的输出缓冲区大小
            // Top to bottom are：image width, image height, input data buffer, input data size, source pixel format,
            // destination pixel format, output data buffer, provided output buffer size
            stConvertParam.nWidth = stImageInfo.nWidth;
            stConvertParam.nHeight = stImageInfo.nHeight;
            stConvertParam.pSrcData = pData;
            stConvertParam.nSrcDataLen = stImageInfo.nFrameLen;
            stConvertParam.enSrcPixelType = stImageInfo.enPixelType;
            stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
            stConvertParam.pDstBuffer = pDataForRGB;
            stConvertParam.nDstBufferSize = stImageInfo.nWidth * stImageInfo.nHeight *  4 + 2048;
            nRet = MV_CC_ConvertPixelType(handle, &stConvertParam);
            if (MV_OK != nRet)
            {
                std::cout<<"MV_CC_ConvertPixelType fail! nRet [%x]"<<nRet<<std::endl;
                break;
            }
            int type =CV_8UC3;
            cv::Mat frameCV(int(stImageInfo.nHeight), int(stImageInfo.nWidth), type, (void *)pDataForRGB);
            cv::Mat frameNew;
            cv::cvtColor(frameCV,frameNew, cv::COLOR_BGR2GRAY);
            memcpy(pDataForRGB, frameNew.data,stImageInfo.nWidth*stImageInfo.nHeight);

            frame.memory = pDataForRGB;
            frame.width = stImageInfo.nWidth;
            frame.height = stImageInfo.nHeight;
            frame.sizeBytes = stImageInfo.nWidth * stImageInfo.nHeight *  4 + 2048;
        }
        else{
            std::cout<<"No data[%x]"<< nRet<<std::endl;
        }
    }

    free(pData);
    // 停止取流
    // end grab image
    nRet = MV_CC_StopGrabbing(handle);
    if (MV_OK != nRet)
    {
       std::cout<<"MV_CC_StopGrabbing fail! nRet [%x]"<< nRet<<std::endl;
        return frame;
    }

    // 关闭设备
    // close device
    nRet = MV_CC_CloseDevice(handle);
    if (MV_OK != nRet)
    {
        std::cout<<"MV_CC_CloseDevice fail! nRet [%x]"<< nRet<<std::endl;
        return frame;
    }

    // 销毁句柄
    // destroy handle
    nRet = MV_CC_DestroyHandle(handle);
    if (MV_OK != nRet)
    {
        std::cout<<"MV_CC_DestroyHandle fail! nRet [%x]"<<nRet<<std::endl;
        return frame;
    }

    return frame;

}

//CameraFrame CamerHikang::getFrame(){

//    CameraFrame frame;

//    //开始采集图像
//    int nRet = MV_CC_StartGrabbing(m_pHandle);
//    if (MV_OK != nRet)
//    {
//        printf("error: StartGrabbing fail [%x]\n", nRet);
//        return frame;
//    }

//    //获取一帧数据的大小
//     MVCC_INTVALUE stIntvalue = {0};
//    nRet = MV_CC_GetIntValue(m_pHandle, "PayloadSize", &stIntvalue);
//    if (nRet != MV_OK)
//    {
//        printf("Get PayloadSize failed! nRet [%x]\n", nRet);
//        return frame;
//    }
//    int nBufSize = stIntvalue.nCurValue; //一帧数据大小

//    unsigned int    nTestFrameSize = 0;
//    unsigned char*  pFrameBuf = NULL;
//    pFrameBuf = (unsigned char*)malloc(nBufSize);

//    MV_FRAME_OUT_INFO_EX   stInfo;
//    memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO));

//    //注册数据回调函数
////    nRet = MV_CC_RegisterImageCallBackEx(m_pHandle, ImageCallBack, NULL);
////    if (MV_OK != nRet)
////    {
////        printf("error: RegisterImageCallBack fail [%x]\n", nRet);
////        return frame;
////    }



////    nRet=MV_CC_TriggerSoftwareExecute(m_pHandle);
////    if(MV_OK != nRet)
////    {
////        printf("");
////    }
//    MV_FRAME_OUT stOutFrame;
//    memset(&stOutFrame, 0, sizeof(MV_FRAME_OUT));

//    nRet = MV_CC_GetImageBuffer(m_pHandle, &stOutFrame, 1000);

//    //上层应用程序需要根据帧率，控制好调用该接口的频率
//    //此次代码仅供参考，实际应用建议另建线程进行图像帧采集和处理
//    nRet = MV_CC_GetOneFrameTimeout(m_pHandle, pFrameBuf, nBufSize, &stInfo, 2000);
//    if (MV_OK != nRet)
//    {
//        printf("MV_CC_GetOneFrameTimeout  retry ,code =%d",nRet);
//        sleep(10);
//        nRet = MV_CC_GetOneFrameTimeout(m_pHandle, pFrameBuf, nBufSize, &stInfo, 2000);
//        if(MV_OK != nRet)
//        {
//            printf("MV_CC_GetOneFrameTimeout  error ,code =%d",nRet);
//        }
//    }

//    //像素格式转换输入输出参数
//    MV_CC_PIXEL_CONVERT_PARAM stParam;
//    memset(&stParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));

//    //源数据
//    stParam.pSrcData       = pFrameBuf;              //原始图像数据
//    stParam.nSrcDataLen    = stInfo.nFrameLen;         //原始图像数据长度
//    stParam.enSrcPixelType = stInfo.enPixelType;       //原始图像数据的像素格式
//    stParam.nWidth         = stInfo.nWidth;            //图像宽
//    stParam.nHeight        = stInfo.nHeight;           //图像高

//    m_nHeight = stInfo.nHeight;
//    m_nWidth = stInfo.nWidth;
//    int  imagesize = stInfo.nWidth*stInfo.nHeight *3; //RGB8
//    //目标数据
//    stParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed;     //
//    stParam.nDstBufferSize = imagesize;             //存储节点的大小
//    unsigned char* pImage  = (unsigned char*)malloc(imagesize);
//    stParam.pDstBuffer     = pImage;                   //输出数据缓冲区，存放转换之后的数据

//    nRet = MV_CC_ConvertPixelType(m_pHandle, &stParam);
//    if(MV_OK != nRet)
//    {
//        printf("MV_CC_ConvertPixelType  error  ret=%d",nRet);
//    }

//    free(pFrameBuf);

//    //停止采集图像
//     nRet = MV_CC_StopGrabbing(m_pHandle);
//    if (MV_OK != nRet)
//    {
//        printf("error: StopGrabbing fail [%x]\n", nRet);
//        return frame;
//    }


//    // Copy frame address and properties
//    frame.memory = pImage;
//    frame.width = stInfo.nWidth;
//    frame.height = stInfo.nHeight;
//    frame.sizeBytes = imagesize;

//    return frame;
//}


size_t CamerHikang::getFrameSizeBytes(){
    if (!capturing) {
        cerr << "ERROR: Cannot get frame size before capturing. Call startCapture() before getFrameSizeBytes()." << endl;
        return 0;
    }

    return m_nHeight*m_nWidth*3;
}

size_t CamerHikang::getFrameWidth(){
    return m_nWidth;
}


size_t CamerHikang::getFrameHeight(){
    return m_nHeight;
}


CamerHikang::~CamerHikang(){
    // Stop camera transmission
    if(capturing)
        stopCapture();


    //关闭设备，释放资源
    int nRet = MV_CC_CloseDevice(m_pHandle);
    if (MV_OK != nRet)
    {
        printf("error: CloseDevice fail [%x]\n", nRet);
        return;
    }

    //销毁句柄，释放资源
     nRet = MV_CC_DestroyHandle(m_pHandle);
    if (MV_OK != nRet)
    {
        printf("error: DestroyHandle fail [%x]\n", nRet);
        return;
    }

}



