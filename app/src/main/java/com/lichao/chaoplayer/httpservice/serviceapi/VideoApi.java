package com.lichao.chaoplayer.httpservice.serviceapi;

import com.lichao.chaoplayer.bean.PandaTvDataBean;
import com.lichao.chaoplayer.bean.PandaTvLiveDataBean;
import com.lichao.chaoplayer.httpservice.service.HttpMethod;
import com.lichao.chaoplayer.httpservice.service.VideoBaseApi;
import io.reactivex.Observable;
import io.reactivex.Observer;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 9:47
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class VideoApi extends VideoBaseApi {

    public static final String BASE_URL_PANDA = "http://api.m.panda.tv/";
    public static VideoApi videoApi;
    public VideoService videoService;

    public VideoApi() {
        videoService = HttpMethod.getInstance(BASE_URL_PANDA).createApi(VideoService.class);
    }

    public static VideoApi getInstance() {
        if(videoApi == null) {
            videoApi = new VideoApi();
        }
        return videoApi;
    }

    public void getVideList(String cate,
                            int pageno,
                            int pagenum,
                            int room,
                            String version,
                            Observer<PandaTvDataBean> subscriber) {
        Observable observable = videoService.getVideList(cate, pageno, pagenum, room, version)
                .map(new HttpResultFunc<PandaTvDataBean>());
        toSubscribe(observable, subscriber);
    }

    public void getLiveUrl(String roomid,
                           String version,
                           Observer<PandaTvLiveDataBean> subscriber) {
        Observable observable = videoService.getLiveUrl(roomid, version, 1, "json", "android")
                .map(new HttpResultFunc<PandaTvLiveDataBean>());
        toSubscribe(observable, subscriber);
    }
}
