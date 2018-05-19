package com.lichao.chaoplayer.bean;

import com.lichao.chaoplayer.base.BaseBean;

import java.util.List;

/**
 * Created by ChaoLi on 2018/5/18 0018 - 19:15
 * Email: lichao3140@gmail.com
 * Version: v1.0  加载视频列表
 */
public class PandaTvDataBean extends BaseBean {

    // http://api.m.panda.tv/ajax_get_live_list_by_cate?cate=lol&pageno=1&pagenum=20&room=1&version=3.3.1.5978

    private List<PandaTvListItemBean> items;

    private String total;

    private Type type;

    private int liveswitch;

    public List<PandaTvListItemBean> getItems() {
        return items;
    }

    public void setItems(List<PandaTvListItemBean> items) {
        this.items = items;
    }

    public String getTotal() {
        return total;
    }

    public void setTotal(String total) {
        this.total = total;
    }

    public Type getType() {
        return type;
    }

    public void setType(Type type) {
        this.type = type;
    }

    public int getLiveswitch() {
        return liveswitch;
    }

    public void setLiveswitch(int liveswitch) {
        this.liveswitch = liveswitch;
    }

    public class Type {
        String ename;

        String cname;

        public String getEname() {
            return ename;
        }

        public void setEname(String ename) {
            this.ename = ename;
        }

        public String getCname() {
            return cname;
        }

        public void setCname(String cname) {
            this.cname = cname;
        }
    }
}
