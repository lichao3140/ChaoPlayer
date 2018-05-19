package com.lichao.chaoplayer.httpservice.service;

import com.lichao.chaoplayer.MyApplication;
import com.lichao.chaoplayer.util.NetUtil;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import okhttp3.Cache;
import okhttp3.CacheControl;
import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Retrofit;
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 9:49
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class HttpMethod {

    public static String token = "";//请求时用户的Token
    private static Retrofit retrofit;

    public static HashMap<String, HttpMethod> urls = new HashMap<>();

    //构造方法私有
    private HttpMethod(String url) {
        retrofit = new Retrofit.Builder()
                .client(genericClient())
                .addConverterFactory(GsonConverterFactory.create())
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
                .baseUrl(url)
                .build();
    }

    public static <T> T createApi(Class<T> clazz) {

        return retrofit.create(clazz);
    }

    //获取单例
    public static HttpMethod getInstance(String url) {
        if(!urls.containsKey(url)) {
            HttpMethod httpMethod = new HttpMethod(url);
            urls.put(url, httpMethod);
        }
        return urls.get(url);
    }

    public static OkHttpClient genericClient() {
        HttpLoggingInterceptor logging = new HttpLoggingInterceptor();
        logging.setLevel(HttpLoggingInterceptor.Level.BODY);
        //设置缓存路径
        File httpCacheDirectory = new File(MyApplication.getInstance().getExternalCacheDir().getAbsolutePath(), "responses");
        //设置缓存 10M
        Cache cache = new Cache(httpCacheDirectory, 50 * 1024 * 1024);
        OkHttpClient httpClient = new OkHttpClient.Builder().addInterceptor(new Interceptor() {
            @Override
            public Response intercept(Chain chain) throws IOException {
                Request request = chain.request().newBuilder().addHeader("token", token).build();
                if (!NetUtil.getNetworkIsConnected(MyApplication.getInstance())) {
                    request = request.newBuilder()
                            .cacheControl(CacheControl.FORCE_CACHE)
                            .build();
                }

                Response response = chain.proceed(request);

                if (NetUtil.getNetworkIsConnected(MyApplication.getInstance())) {
                    int maxAge = 0 * 60; // 有网络时 设置缓存超时时间0个小时
                    response.newBuilder()
                            .addHeader("Cache-Control", "public, max-age=" + maxAge)
                            .removeHeader("Pragma")// 清除头信息，因为服务器如果不支持，会返回一些干扰信息，不清除下面无法生效
                            .build();
                } else {
                    int maxStale = 60 * 60 * 24 * 7; // 无网络时，设置超时为1周
                    response.newBuilder()
                            .addHeader("Cache-Control", "public, only-if-cached, max-stale=" + maxStale)
                            .removeHeader("Pragma")
                            .build();
                }
                return response;
            }
        }).addInterceptor(logging).cache(cache).build();
        return httpClient;
    }

}
