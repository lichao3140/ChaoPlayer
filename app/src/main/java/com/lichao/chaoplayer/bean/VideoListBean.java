package com.lichao.chaoplayer.bean;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 20:42
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class VideoListBean {

    private String parent;
    private String path;
    private String name;
    private boolean isFile;

    public boolean isFile() {
        return isFile;
    }

    public void setFile(boolean file) {
        isFile = file;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getParent() {
        return parent;
    }

    public void setParent(String parent) {
        this.parent = parent;
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }
}
