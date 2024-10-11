#include "varpaint.h"

htparams::htparams(int l, int r, int p0, int p1, int p2, int p3, double fl, double fe, QString text){
    L = l;
    R = r;
    Plan =  p0;
    PlanL = p1;
    PlanM = p2;
    PlanR = p3;
    Fl = fl;
    Fr = fe;
    desc = text;
}

htparams::htparams(QJsonObject data){
    setVal(data);
}

bool htparams::operator ==(const htparams &o){
    return L == o.L &&
           R == o.R &&
           Plan == o.Plan &&
           PlanL == o.PlanL &&
           PlanM == o.PlanM &&
           PlanR == o.PlanR &&
           Fl == o.Fl &&
           Fr == o.Fr &&
           desc == o.desc;
}

bool htparams::compare_rays(htparams &st1, htparams &st2){
    return st1.L<st2.L;
}

bool htparams::compare_area(htparams &st1, htparams &st2){
    return st1.R<st2.R;
}

QJsonObject htparams::json(){
    QJsonObject data;
    data["L"] = (int)L;
    data["R"] = (int)R;
    data["P"] = (int)Plan;
    data["PL"] = (int)PlanL;
    data["PM"] = (int)PlanM;
    data["PR"] = (int)PlanR;
    data["Fl"] = (double)Fl;
    data["Fr"] = (double)Fr;
    data["Desc"] = (QString)desc;
    return data;
}

void htparams::setVal(QJsonObject data){
    L = data["L"].toInt();
    R = data["R"].toInt();
    Plan = data["P"].toInt();
    PlanL = data["PL"].toInt();
    PlanM = data["PM"].toInt();
    PlanR = data["PR"].toInt();
    Fl =  data["Fl"].toDouble();
    Fr =  data["Fr"].toDouble();
    desc =  data["Desc"].toString();
}

Point::Point(int w, int h){
    this->w = w;
    this->h = h;
    w_ = w;
    h_ = h;
}

bool Point::bear(int rw, int rh){
    return (w_>=(rw-3) && (w_<=(rw+3)) && h_>=(rh-3) &&(h_<=(rh+3)));
}

Point Point::mediana(int nw, int nh){
    Point p((nw-w)/2,(nh-h)/2);
    return p;
}

ulong Point::distanceSqrd(int w, int h){
    int wd = w-this->w;
    int hd = h-this->h;
    return ((ulong)wd*(ulong)wd)+((ulong)hd*(ulong)hd);
}

Point Point::summ(int nw, int nh,int bw,int bh){
    float f=(float)bh/(float)bw;
    Point p(((float)(nw-w)/f)+nw,f*((float)(nh-h))+nh);
    return p;
}

Point Point::simetr(float f){
    float a1=1.0/f;
    float b1=0.0;
    float a2=-(1/a1);

    float b2=h-a2*w;

    int w1 = (int)((b2-b1) / (a1-a2));
    int h1 = (int)((1.0/f) * (float) w1);
    int w2 = 2 * w1 - w;
    int h2 = 2 * h1 - h;

    Point p(w2,h2);
    return p;
}

bool Point::isGood(){
    return !(w<=0 ||h<=0);
}

bool Point::operator==(const Point &other) const{
    return (w==other.w)&&(h==other.h);
}

PlanMap::PlanMap(int w, int h){
    width_ = w;
    height_ = h;

    QVector<int> t(h,0);
    for (int i = 0; i < w; ++i) {
        map.append(t);
    }
}

PlanMap::~PlanMap(){}

void PlanMap::SetPlan(int w, int h, int plan){
    if(w>=width_) return;
    if(h>=height_) return;
    map[w][h] = plan;
}

int PlanMap::width(){
    return width_;
}

int PlanMap::height(){
    return height_;
}

int PlanMap::get(int w, int h){
    if(w >= width_) return 0;
    if(h >= height_) return 0;
    return map[w][h];
}

void tcHT::multData(double value){
    forward = forward*value;
    backward = backward*value;
    coefCalc();
}

void tcHT::setKS(int value){
    csht = value;
    if(forward==0 && backward==0)
        csht = 1;
}

void tcHT::coefCalc(){
    if(forward == 0) forward = 1;
    if(backward == 0) backward = 1;
    coef = (float)backward/(float)forward;
}

Voronoi::Voronoi(int height,int width){
    delH=1.0;
    delW=1.0;

    this->width=width;
    this->height=height;

    planMap = new PlanMap(this->width,this->height);

    sprayes.clear();
    bogoses.clear();
}

QRectF Voronoi::get_rect_area(){
    return QRect(0,0,width,height);
}

int Voronoi::RandomBetween(int low, int high){
    return (QRandomGenerator::global()->generate() % ((high + 1) - low) + low);
}

void Voronoi::addRays(htparams st){
    Point p(st.R,st.L);
    p.w = p.w*delW;
    p.h = p.h*delH;
    points.append(p);
    st.L = st.L*delH;
    st.R = st.R*delW;
    sts.append(st);
}

void Voronoi::addArea(htparams ar){
    Point p(ar.R,ar.L);
    p.w = p.w*delW;
    p.h = p.h*delH;
    points.append(p);
    plans.append(ar.Plan);
    sts.append(ar);
}

void Voronoi::addBogoses(QVector<Point> bogs){
    bogoses.clear();
    foreach(Point p,bogs){
        p.w=p.w*delW;
        p.h=p.h*delH;
        bogoses.append(p);
    }
}

void Voronoi::addSpray(Point point, QString name){
    point.w = point.w*delW;
    point.h = point.h*delH;

    point.name = name;

    foreach(auto s_point,sprayes){
        if(s_point == point){
            sprayes.removeOne(s_point);
        }
    }

    sprayes.append(point);
}
void Voronoi::clearSpray(){
    sprayes.clear();
    bogoses.clear();
}

void Voronoi::makeDiagramm(){
    std::sort(sts.begin(),sts.end(),htparams::compare_area);

    float mxw = 1;
    float mxh = 1;

    if(!sts.isEmpty()){
        mxw = sts.last().R;
        mxh = sts.last().L;
    }

    for (int hh = 1; hh < mxh; ++hh) {
        for (int ww = 1; ww < mxw; ++ww) {
            foreach (auto s, sts) {
                if(ww<=s.R&&hh<=s.L){
                    int plan=s.PlanM;
                    float f=((float)ww/(float)hh);
                    if(f<=s.Fl) plan=s.PlanL;
                    if(f>=s.Fr) plan=s.PlanR;
                    planMap->SetPlan(ww,hh,plan);
                    break;
                }
            }
        }
    }

    for(int i=0; i<planMap->map.size(); ++i){
        std::reverse(planMap->map[i].begin(),planMap->map[i].end());
    }

    /* переопределение областей планов */
    QList<int> maps;
    int cur_plan;
    for(int h=0; h<planMap->height(); h++){
        for(int w=0; w<planMap->width(); w++){
            cur_plan = planMap->get(h,w);
            if(cur_plan != 0){
                if(!maps.contains(planMap->get(h,w))){
                    maps.append(cur_plan);
                }
            }
        }
    }

    QList<QList<QPoint>> list_areas;
    for(int i=0; i<maps.size(); i++){
        list_areas.append(QList<QList<QPoint>>());
    }

    foreach(int plan, maps){
        QList<QPoint> conture;
        for (int ww = 1; ww < planMap->width(); ++ww) {
            for (int hh = 1; hh < planMap->height(); ++hh) {
                if(plan == planMap->get(ww,hh)){
                    conture.append(QPoint(ww,hh));
                    break;
                }
            }
        }
        for (int ww = planMap->width(); ww >= 1; --ww) {
            for (int hh = planMap->height(); hh >= 1; --hh) {
                if(plan == planMap->get(ww,hh)){
                    conture.append(QPoint(ww,hh));
                    break;
                }
            }
        }
        listPlans.insert(plan,conture);
    }
}

void Voronoi::makeAreals(){
    ulong d;
    for (int hh = 0; hh < height; ++hh) {
        for (int ww = 0; ww < width; ++ww) {
            int ind=-1;
            ulong dist = INT_MAX;
            for (int it=0;it<points.size();++it){
                d=points[it].distanceSqrd(ww,hh);
                if (d<dist){
                    dist=d;
                    ind=it;
                }
            }
            if(ind>-1){
                planMap->SetPlan(ww,hh,plans[ind]);
            }
        }
    }

    /* переопределение областей планов */
    QList<int> maps;
    int cur_plan;
    for(int h=0; h<planMap->height(); h++){
        for(int w=0; w<planMap->width(); w++){
            cur_plan = planMap->get(h,w);
            if(cur_plan != 0){
                if(!maps.contains(planMap->get(h,w))){
                    maps.append(cur_plan);
                }
            }
        }
    }

    QList<QList<QPoint>> list_areas;
    for(int i=0; i<maps.size(); i++){
        list_areas.append(QList<QList<QPoint>>());
    }

    foreach(int plan, maps){
        QList<QPoint> conture;
        for (int ww = 1; ww < planMap->width(); ++ww) {
            for (int hh = 1; hh < planMap->height(); ++hh) {
                if(plan == planMap->get(ww,hh)){
                    conture.append(QPoint(ww,hh));
                    break;
                }
            }
        }
        for (int ww = planMap->width(); ww >= 1; --ww) {
            for (int hh = planMap->height(); hh >= 1; --hh) {
                if(plan == planMap->get(ww,hh)){
                    conture.append(QPoint(ww,hh));
                    break;
                }
            }
        }
        listPlans.insert(plan,conture);
    }
}

int Voronoi::GetPlan(int w, int h){
    return planMap->get(w/delW,h/delH);
}

QVector<Point> Voronoi::createPoints(int count){
    QVector<Point> points;
    for (int i = 0; i < count; i++) {
        points.push_back({ RandomBetween(0,width) , RandomBetween(0,height) });
    }
    return points;
}

bool Voronoi::compare(const QPoint &first, const QPoint &second){
    if (first.x() < second.x()){
        return true;
    }else if (first.x() > second.x()){
        return false;
    }else{
        if (first.y() > second.y()){
            return true;
        }else{
            return false;
        }
    }
}

chan::chan(QVector<int> fw, QVector<int> bw, QString desct){
    f=fw;
    b=bw;
    desc=desct;
}

chan::chan(QJsonObject data){
    setVal(data);
}

QString chan::fin(){
    QString r;
    for(int i=0; i<f.size(); ++i){
        r += QString::number(f[i]) + ";";
    }
    return r;
}

QString chan::bin(){
    QString r;
    for(int i=0; i<b.size(); ++i){
        r += QString::number(b[i]) + ";";
    }
    return r;
}

bool chan::operator ==(const chan &o){
    return f == o.f &&
           b == o.b &&
           desc == o.desc;
}

QJsonObject chan::json(){
    QJsonObject data;

    QJsonArray fja;
    for(int i=0; i<f.size(); ++i){
        fja.push_back(f[i]);
    }
    QJsonArray bja;
    for(int i=0; i<b.size(); ++i){
        bja.push_back(b[i]);
    }
    data["f"] = fja;
    data["b"] = bja;
    data["desc"] = (QString)desc;
    return data;
}

void chan::setVal(QJsonObject data){
    f.clear();
    QJsonArray far = data["f"].toArray();
    for(int i=0; i<far.size(); ++i){
        f.push_back(far[i].toInt());
    }

    b.clear();
    QJsonArray bar = data["b"].toArray();
    for(int i=0; i<bar.size(); ++i){
        b.push_back(bar[i].toInt());
    }
    desc = data["desc"].toString();
}
