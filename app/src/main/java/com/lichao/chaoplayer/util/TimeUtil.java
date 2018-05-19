package com.lichao.chaoplayer.util;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 12:56
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class TimeUtil {

    public static String secdsToDateFormat(int sends) {
        if(sends <= 0) {
            return "00:00:00";
        }
        long hours = sends / (60 * 60);
        long minutes = (sends % (60 * 60)) / (60);
        long seconds = sends % (60);

        String sh = "00";
        if (hours > 0) {
            if (hours < 10) {
                sh = "0" + hours;
            } else {
                sh = hours + "";
            }
        }

        String sm = "00";
        if (minutes > 0) {
            if (minutes < 10) {
                sm = "0" + minutes;
            } else {
                sm = minutes + "";
            }
        }

        String ss = "00";
        if (seconds > 0) {
            if (seconds < 10) {
                ss = "0" + seconds;
            } else {
                ss = seconds + "";
            }
        }
        return sh + ":" + sm + ":" + ss;
    }
}
