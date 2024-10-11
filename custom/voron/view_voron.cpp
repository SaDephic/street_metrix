#include "view_voron.h"

view_voron::view_voron(){
    vor_item = new voron_item;
}

void view_voron::set_current_position(QPointF cur_pos){
    if(vor_item->v){
        int w=cur_pos.x();
        int h=vor_item->v->planMap->height()-cur_pos.y();
        if(w>=0&&w<vor_item->v->planMap->width() &&h>=0&&h<vor_item->v->planMap->height()){
            QString res = QString();
            for(int i=0; i<vor_item->v->sprayes.size(); i++){
                Point p = vor_item->v->sprayes[i];
                if(h==p.h && w==p.w)
                    res = QVariant(p.h).toString()+":"+QVariant(p.w).toString()+"->"+p.name;
            }
            if(res.isEmpty())
                emit current_position_data(QVariant(h).toString() + ":" + QVariant(w).toString() + " " + QString::asprintf("%.2f",(float)w/(float)h));
            else
                emit current_position_data(res);
        }else{
            emit current_position_data(QString());
        }
    }
}
