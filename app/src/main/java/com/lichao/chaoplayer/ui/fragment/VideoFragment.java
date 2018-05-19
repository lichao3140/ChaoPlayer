package com.lichao.chaoplayer.ui.fragment;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.lichao.chaoplayer.R;
import com.lichao.chaoplayer.adapter.PandaVideoListAdapter;
import com.lichao.chaoplayer.adapter.WapHeaderAndFooterAdapter;
import com.lichao.chaoplayer.base.BaseFragment;
import com.lichao.chaoplayer.bean.PandaTvDataBean;
import com.lichao.chaoplayer.bean.PandaTvListItemBean;
import com.lichao.chaoplayer.bean.PandaTvLiveDataBean;
import com.lichao.chaoplayer.httpservice.serviceapi.VideoApi;
import com.lichao.chaoplayer.httpservice.subscribers.HttpSubscriber;
import com.lichao.chaoplayer.httpservice.subscribers.SubscriberOnListener;
import com.lichao.chaoplayer.ui.activity.VideoLiveActivity;
import com.lichao.chaoplayer.util.MyLog;

import java.util.ArrayList;
import java.util.List;

import butterknife.BindView;

/**
 * Created by ChaoLi on 2018/5/18 0018 - 12:48
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class VideoFragment extends BaseFragment {

    @BindView(R.id.recyclerview)
    RecyclerView recyclerView;
    @BindView(R.id.swipRefresh)
    SwipeRefreshLayout swipeRefreshLayout;

    private TextView tvLoadMsg;

    private PandaVideoListAdapter pandaVideoListAdapter;
    private WapHeaderAndFooterAdapter headerAndFooterAdapter;
    private List<PandaTvListItemBean> datas;

    private int currentPage = 1;
    private int pageSize = 20;
    private boolean isLoad = true;
    private boolean isOver = false;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.fragment_video_layout);
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        initAdapter();
        getVideoList();
        // 下拉刷新
        swipeRefreshLayout.setColorSchemeColors(getResources().getColor(R.color.color_ec4c48));
        swipeRefreshLayout.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                if(!isLoad)
                {
                    isLoad = true;
                    currentPage = 1;
                    getVideoList();
                }
            }
        });
    }

    private void initAdapter() {
        datas = new ArrayList<>();
        pandaVideoListAdapter = new PandaVideoListAdapter(getActivity(), datas);
        headerAndFooterAdapter = new WapHeaderAndFooterAdapter(pandaVideoListAdapter);

        GridLayoutManager gridLayoutManager = new GridLayoutManager(getActivity(), 2);
        gridLayoutManager.setOrientation(GridLayoutManager.VERTICAL);
        recyclerView.setLayoutManager(gridLayoutManager);

        View footerView = LayoutInflater.from(getActivity()).inflate(R.layout.footer_layout, recyclerView, false);
        tvLoadMsg = footerView.findViewById(R.id.tv_loadmsg);
        headerAndFooterAdapter.addFooter(footerView);

        recyclerView.setAdapter(headerAndFooterAdapter);

        pandaVideoListAdapter.setOnItemClickListener(new PandaVideoListAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(PandaTvListItemBean pandaTvListItemBean) {
                showLoadDialog("加载中");
                getLiveUrl(pandaTvListItemBean.getId(), "3.3.1.5978");
            }
        });

        headerAndFooterAdapter.setOnloadMoreListener(recyclerView, new WapHeaderAndFooterAdapter.OnLoadMoreListener(){

            @Override
            public void onLoadMore() {
                if(isOver) {
                    return;
                } if(!isLoad) {
                    isLoad = true;
                    getVideoList();
                }
            }
        });
    }

    private void getVideoList() {
        VideoApi.getInstance().getVideList("lol",
                currentPage,
                pageSize,
                1,
                "3.3.1.5978",
                new HttpSubscriber<PandaTvDataBean>(new SubscriberOnListener<PandaTvDataBean>() {
                    @Override
                    public void onSucceed(PandaTvDataBean data) {
                        if (currentPage == 1) {
                            datas.clear();
                        }

                        if (data.getItems().size() < pageSize) {
                            tvLoadMsg.setText("没有更多了");
                            isOver = true;
                        } else {
                            tvLoadMsg.setText("加载更多");
                            isOver = false;
                            currentPage++;
                        }

                        datas.addAll(data.getItems());
                        headerAndFooterAdapter.notifyDataSetChanged();
                        isLoad = false;
                        swipeRefreshLayout.setRefreshing(false);
                    }

                    @Override
                    public void onError(int code, String msg) {
                        MyLog.d(msg);
                        isLoad = false;
                        swipeRefreshLayout.setRefreshing(false);
                    }
                }, getActivity()));
    }

    private void getLiveUrl(String roomId, String version) {
        VideoApi.getInstance().getLiveUrl(roomId, version, new HttpSubscriber<PandaTvLiveDataBean>(new SubscriberOnListener<PandaTvLiveDataBean>() {
            @Override
            public void onSucceed(PandaTvLiveDataBean data) {
                String[] pl = data.getInfo().getVideoinfo().getPlflag().split("_");

                if(pl != null && pl.length > 0) {
                    String url = "http://pl" + pl[pl.length - 1] + ".live.panda.tv/live_panda/" + data.getInfo().getVideoinfo().getRoom_key() + "_mid.flv?sign=" + data.getInfo().getVideoinfo().getSign() + "&time=" + data.getInfo().getVideoinfo().getTs();
                    MyLog.d(url);
                    Bundle bundle = new Bundle();
                    bundle.putString("url", url);
                    VideoLiveActivity.startActivity(getActivity(), VideoLiveActivity.class, bundle);
                }
                hideLoadDialog();
            }

            @Override
            public void onError(int code, String msg) {
                hideLoadDialog();
                showToast(msg);
            }
        }, getActivity()));
    }
}
