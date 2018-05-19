package com.lichao.chaoplayer.bean;

import com.lichao.chaoplayer.base.BaseBean;
import java.util.List;

/**
 * Created by ChaoLi on 2018/5/18 0018 - 19:03
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class PandaTvListItemBean extends BaseBean {
    /**
     * id : 371037
     * name : MSI半决赛RNG vs FNC
     * hostid : 31933058
     * person_num : 22541411
     * classification : {"cname":"英雄联盟","ename":"lol"}
     * pictures : {"img":"http://i9.pdim.gs/8f42fcb505782ee65e4e80d03d4ce3fa.png"}
     * display_type : 1
     * tag : 蓝光
     * tag_switch : 4
     * tag_color : 2
     * style_type : 1
     * reliable : 1
     * status : 2
     * stream_status : 1
     * createtime : 2016-04-12 12:00:15
     * start_time : 1514475812
     * schedule : 0
     * room_type : 1
     * lianmai : 3
     * host_level_info : {"val":9308.144778,"c_lv":16,"c_lv_val":8029,"n_lv":17,"n_lv_val":9996,"plays_day":222,"bamboo_user":1967.409414,"gift_user":3527.9639340000003,"gift_cnt":3590.985579,"vip":1}
     * top_icon : 0
     * label : []
     * room_key : dd9f182bcec99d04099113e618cfc5b3
     * fans : 0
     * userinfo : {"rid":31933058,"userName":"","nickName":"2018MSI季中冠军赛","avatar":"http://i9.pdim.gs/c02c9779227f3ca106c0883a9c9eff0f.png"}
     * announcement :
     * duration : 12165282
     * click_trace : list
     * room_activity : {"type":"1","value":"蓝光"}
     * definition_option : {"HD":"1","OD":"1","SD":"1"}
     * xy_stat : 0
     * tx_stat : 0
     * hardware : 2
     * decoder : {"HD":"","OD":"","SD":""}
     */

    private String id;
    private String name;
    private String hostid;
    private String person_num;
    private ClassificationBean classification;
    private PicturesBean pictures;
    private String display_type;
    private String tag;
    private String tag_switch;
    private String tag_color;
    private String style_type;
    private String reliable;
    private String status;
    private String stream_status;
    private String createtime;
    private String start_time;
    private String schedule;
    private String room_type;
    private String lianmai;
    private String host_level_info;
    private String top_icon;
    private String room_key;
    private String fans;
    private UserinfoBean userinfo;
    private String announcement;
    private String duration;
    private String click_trace;
    private RoomActivityBean room_activity;
    private DefinitionOptionBean definition_option;
    private String xy_stat;
    private String tx_stat;
    private String hardware;
    private DecoderBean decoder;
    private List<?> label;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getHostid() {
        return hostid;
    }

    public void setHostid(String hostid) {
        this.hostid = hostid;
    }

    public String getPerson_num() {
        return person_num;
    }

    public void setPerson_num(String person_num) {
        this.person_num = person_num;
    }

    public ClassificationBean getClassification() {
        return classification;
    }

    public void setClassification(ClassificationBean classification) {
        this.classification = classification;
    }

    public PicturesBean getPictures() {
        return pictures;
    }

    public void setPictures(PicturesBean pictures) {
        this.pictures = pictures;
    }

    public String getDisplay_type() {
        return display_type;
    }

    public void setDisplay_type(String display_type) {
        this.display_type = display_type;
    }

    public String getTag() {
        return tag;
    }

    public void setTag(String tag) {
        this.tag = tag;
    }

    public String getTag_switch() {
        return tag_switch;
    }

    public void setTag_switch(String tag_switch) {
        this.tag_switch = tag_switch;
    }

    public String getTag_color() {
        return tag_color;
    }

    public void setTag_color(String tag_color) {
        this.tag_color = tag_color;
    }

    public String getStyle_type() {
        return style_type;
    }

    public void setStyle_type(String style_type) {
        this.style_type = style_type;
    }

    public String getReliable() {
        return reliable;
    }

    public void setReliable(String reliable) {
        this.reliable = reliable;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getStream_status() {
        return stream_status;
    }

    public void setStream_status(String stream_status) {
        this.stream_status = stream_status;
    }

    public String getCreatetime() {
        return createtime;
    }

    public void setCreatetime(String createtime) {
        this.createtime = createtime;
    }

    public String getStart_time() {
        return start_time;
    }

    public void setStart_time(String start_time) {
        this.start_time = start_time;
    }

    public String getSchedule() {
        return schedule;
    }

    public void setSchedule(String schedule) {
        this.schedule = schedule;
    }

    public String getRoom_type() {
        return room_type;
    }

    public void setRoom_type(String room_type) {
        this.room_type = room_type;
    }

    public String getLianmai() {
        return lianmai;
    }

    public void setLianmai(String lianmai) {
        this.lianmai = lianmai;
    }

    public String getHost_level_info() {
        return host_level_info;
    }

    public void setHost_level_info(String host_level_info) {
        this.host_level_info = host_level_info;
    }

    public String getTop_icon() {
        return top_icon;
    }

    public void setTop_icon(String top_icon) {
        this.top_icon = top_icon;
    }

    public String getRoom_key() {
        return room_key;
    }

    public void setRoom_key(String room_key) {
        this.room_key = room_key;
    }

    public String getFans() {
        return fans;
    }

    public void setFans(String fans) {
        this.fans = fans;
    }

    public UserinfoBean getUserinfo() {
        return userinfo;
    }

    public void setUserinfo(UserinfoBean userinfo) {
        this.userinfo = userinfo;
    }

    public String getAnnouncement() {
        return announcement;
    }

    public void setAnnouncement(String announcement) {
        this.announcement = announcement;
    }

    public String getDuration() {
        return duration;
    }

    public void setDuration(String duration) {
        this.duration = duration;
    }

    public String getClick_trace() {
        return click_trace;
    }

    public void setClick_trace(String click_trace) {
        this.click_trace = click_trace;
    }

    public RoomActivityBean getRoom_activity() {
        return room_activity;
    }

    public void setRoom_activity(RoomActivityBean room_activity) {
        this.room_activity = room_activity;
    }

    public DefinitionOptionBean getDefinition_option() {
        return definition_option;
    }

    public void setDefinition_option(DefinitionOptionBean definition_option) {
        this.definition_option = definition_option;
    }

    public String getXy_stat() {
        return xy_stat;
    }

    public void setXy_stat(String xy_stat) {
        this.xy_stat = xy_stat;
    }

    public String getTx_stat() {
        return tx_stat;
    }

    public void setTx_stat(String tx_stat) {
        this.tx_stat = tx_stat;
    }

    public String getHardware() {
        return hardware;
    }

    public void setHardware(String hardware) {
        this.hardware = hardware;
    }

    public DecoderBean getDecoder() {
        return decoder;
    }

    public void setDecoder(DecoderBean decoder) {
        this.decoder = decoder;
    }

    public List<?> getLabel() {
        return label;
    }

    public void setLabel(List<?> label) {
        this.label = label;
    }

    public static class ClassificationBean {
        /**
         * cname : 英雄联盟
         * ename : lol
         */

        private String cname;
        private String ename;

        public String getCname() {
            return cname;
        }

        public void setCname(String cname) {
            this.cname = cname;
        }

        public String getEname() {
            return ename;
        }

        public void setEname(String ename) {
            this.ename = ename;
        }
    }

    public static class PicturesBean {
        /**
         * img : http://i9.pdim.gs/8f42fcb505782ee65e4e80d03d4ce3fa.png
         */

        private String img;

        public String getImg() {
            return img;
        }

        public void setImg(String img) {
            this.img = img;
        }
    }

    public static class UserinfoBean {
        /**
         * rid : 31933058
         * userName :
         * nickName : 2018MSI季中冠军赛
         * avatar : http://i9.pdim.gs/c02c9779227f3ca106c0883a9c9eff0f.png
         */

        private int rid;
        private String userName;
        private String nickName;
        private String avatar;

        public int getRid() {
            return rid;
        }

        public void setRid(int rid) {
            this.rid = rid;
        }

        public String getUserName() {
            return userName;
        }

        public void setUserName(String userName) {
            this.userName = userName;
        }

        public String getNickName() {
            return nickName;
        }

        public void setNickName(String nickName) {
            this.nickName = nickName;
        }

        public String getAvatar() {
            return avatar;
        }

        public void setAvatar(String avatar) {
            this.avatar = avatar;
        }
    }

    public static class RoomActivityBean {
        /**
         * type : 1
         * value : 蓝光
         */

        private String type;
        private String value;

        public String getType() {
            return type;
        }

        public void setType(String type) {
            this.type = type;
        }

        public String getValue() {
            return value;
        }

        public void setValue(String value) {
            this.value = value;
        }
    }

    public static class DefinitionOptionBean {
        /**
         * HD : 1
         * OD : 1
         * SD : 1
         */

        private String HD;
        private String OD;
        private String SD;

        public String getHD() {
            return HD;
        }

        public void setHD(String HD) {
            this.HD = HD;
        }

        public String getOD() {
            return OD;
        }

        public void setOD(String OD) {
            this.OD = OD;
        }

        public String getSD() {
            return SD;
        }

        public void setSD(String SD) {
            this.SD = SD;
        }
    }

    public static class DecoderBean {
        /**
         * HD :
         * OD :
         * SD :
         */

        private String HD;
        private String OD;
        private String SD;

        public String getHD() {
            return HD;
        }

        public void setHD(String HD) {
            this.HD = HD;
        }

        public String getOD() {
            return OD;
        }

        public void setOD(String OD) {
            this.OD = OD;
        }

        public String getSD() {
            return SD;
        }

        public void setSD(String SD) {
            this.SD = SD;
        }
    }
}
