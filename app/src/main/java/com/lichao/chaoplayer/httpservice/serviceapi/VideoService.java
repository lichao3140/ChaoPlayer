package com.lichao.chaoplayer.httpservice.serviceapi;

import com.lichao.chaoplayer.bean.PandaTvDataBean;
import com.lichao.chaoplayer.bean.PandaTvLiveDataBean;
import com.lichao.chaoplayer.httpservice.httpentity.VideoHttpResult;

import io.reactivex.Observable;
import retrofit2.http.GET;
import retrofit2.http.Query;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 8:38
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public interface VideoService {

    // http://api.m.panda.tv/ajax_get_live_list_by_cate?cate=lol&pageno=1&pagenum=20&room=1&version=3.3.1.5978
    @GET("ajax_get_live_list_by_cate")
    Observable<VideoHttpResult<PandaTvDataBean>> getVideList(@Query("cate") String cate,
                                                             @Query("pageno") int pageno,
                                                             @Query("pagenum") int pagenum,
                                                             @Query("room") int room,
                                                             @Query("version") String version);


    // http://api.m.panda.tv/ajax_get_liveroom_baseinfo?roomid=406001&__version=3.3.1.5978&slaveflag=1&type=json&__plat=android
    @GET("ajax_get_liveroom_baseinfo")
    Observable<VideoHttpResult<PandaTvLiveDataBean>> getLiveUrl(@Query("roomid") String roomid,
                                                                @Query("__version") String version,
                                                                @Query("slaveflag") int slaveflag,
                                                                @Query("type") String type,
                                                                @Query("__plat") String __plat);
}
