package com.lichao.chaoplayer.adapter;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import com.lichao.chaoplayer.R;
import com.lichao.chaoplayer.bean.VideoListBean;
import java.util.List;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 20:39
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class VideoListAdapter extends RecyclerView.Adapter<VideoListAdapter.TypeHolder> {

    private Context context;
    private List<VideoListBean> datas;
    private OnItemClickListener onItemClickListener;

    public void setOnItemClickListener(OnItemClickListener onItemClickListener) {
        this.onItemClickListener = onItemClickListener;
    }

    public VideoListAdapter(Context context, List<VideoListBean> datas) {
        this.context = context;
        this.datas = datas;
    }

    @NonNull
    @Override
    public TypeHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(context).inflate(R.layout.item_videlist_layout, parent, false);
        TypeHolder holder = new TypeHolder(view);
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull TypeHolder holder, final int position) {
        holder.tvName.setText(datas.get(position).getName());
        holder.tvName.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(onItemClickListener != null)
                {
                    onItemClickListener.onItemClick(datas.get(position));
                }
            }
        });
    }

    @Override
    public int getItemCount() {
        return datas.size();
    }

    public class TypeHolder extends RecyclerView.ViewHolder {

        private TextView tvName;
        public TypeHolder(View itemView) {
            super(itemView);
            tvName = itemView.findViewById(R.id.tv_name);
        }
    }

    public interface OnItemClickListener {
        void onItemClick(VideoListBean videoListBean);
    }
}
