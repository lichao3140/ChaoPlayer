package com.lichao.chaoplayer.base;

import android.support.v4.app.Fragment;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import com.lichao.chaoplayer.R;
import com.lichao.chaoplayer.ui.dialog.LoadingDialog;
import com.lichao.chaoplayer.util.CommonUtil;
import butterknife.BindView;
import butterknife.ButterKnife;

/**
 * Created by ChaoLi on 2018/5/17 0017 - 21:45
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public abstract class BaseFragment extends Fragment {

    public View contentView;
    protected int layoutResId;

    @Nullable
    @BindView(R.id.tv_title)
    TextView mtvTitle;
    @Nullable
    @BindView(R.id.ly_system_bar)
    LinearLayout lySystemBar;

    private LayoutInflater mlayoutInflate;
    LoadingDialog loadingDialog;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        mlayoutInflate = LayoutInflater.from(getActivity());
        contentView = inflater.inflate(layoutResId, container, false);
        ButterKnife.bind(this, contentView);
        if (lySystemBar != null) {
            initSystembar(lySystemBar);
        }
        return contentView;
    }

    public void initSystembar(View lySystemBar) {
        if (Build.VERSION.SDK_INT >= 19) {
            if (lySystemBar != null) {
                lySystemBar.setVisibility(View.VISIBLE);
                LinearLayout.LayoutParams lp = (LinearLayout.LayoutParams) lySystemBar.getLayoutParams();
                lp.height = CommonUtil.getStatusHeight(getActivity());
                lySystemBar.requestLayout();
            }
        } else {
            if (lySystemBar != null) {
                lySystemBar.setVisibility(View.GONE);
            }
        }
    }

    public void setContentView(int layoutResId) {
        this.layoutResId = layoutResId;
    }

    /**
     * 设置标题
     *
     * @param title
     */
    public void setTitle(String title) {
        if (mtvTitle != null && !TextUtils.isEmpty(title)) {
            mtvTitle.setText(title);
        }
    }

    public void showToast(String msg) {
        Toast.makeText(getActivity(), msg, Toast.LENGTH_SHORT).show();
    }

    public void showLoadDialog(String msg) {
        if (loadingDialog == null) {
            loadingDialog = new LoadingDialog(getActivity());
        }
        loadingDialog.show();
        loadingDialog.setMsg(msg);
    }

    public void hideLoadDialog() {
        if (loadingDialog != null) {
            loadingDialog.dismiss();
        }
    }
}
