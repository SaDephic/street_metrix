#ifndef QUERYS_H
#define QUERYS_H

#include <QString>

struct querys{
    querys(){}
    //data all regions
    QString a_region = "SELECT * FROM public.region;";
    //date cross
    QString a_cro_per = "SELECT region,area,id,describ,subarea,((state->'arrays')::jsonb->'defstatis')::jsonb "
                        "AS interval "
                        "FROM public.cross "
                        "WHERE region=%1 and area=%2;";
    //full statistic by date
    QString a_statistic = "SELECT jsonb_insert(to_jsonb(a.def_stat),'{Statistics,0}',to_jsonb(COALESCE(to_jsonb(b.added),'{}'::jsonb))) AS stat "
                          "FROM ("
                          "SELECT id, stat as def_stat FROM public.statistics "
                          "WHERE region=%1 AND area=%2 AND id=%3 AND date='%4')a "
                          "LEFT JOIN ("
                          "SELECT id, jsonb_set(jsonb_path_query(stat,'$.Statistics[*] ? (@.Hour == $H && @.Min == $M)','{\"H\":0,\"M\":0}'),'{Hour}', to_jsonb(24), false) AS added "
                          "FROM public.statistics "
                          "WHERE region=%1 AND area=%2 AND id=%3 AND date='%4')b "
                          "ON a.id = b.id;";
    //dates of month
    QString a_monthstat = "SELECT region, area, id, date "
                          "FROM public.statistics "
                          "WHERE region=%1 AND area=%2 AND ID=%3 AND EXTRACT(month FROM date)=%4 AND EXTRACT(year FROM date)=%5 "
                          "ORDER BY date;";
    //data of selected date
    QString a_monthstatdata = "SELECT jsonb_insert(to_jsonb(a.def_stat),'{Statistics,0}',to_jsonb(COALESCE(to_jsonb(b.added),'{}'::jsonb))) AS stat "
                              "FROM ( "
                              "SELECT id, stat as def_stat FROM public.statistics "
                              "WHERE region=%1 AND area=%2 AND ID=%3 AND date = '%4' "
                              "ORDER BY date)a "
                              "LEFT JOIN ( "
                              "SELECT id, jsonb_set(jsonb_path_query(stat,'$.Statistics[*] ? (@.Hour == $H && @.Min == $M)','{\"H\":0,\"M\":0}'),'{Hour}', to_jsonb(24), false) AS added "
                              "FROM public.statistics "
                              "WHERE region=%1 AND area=%2 AND ID=%3 AND date = '%5' "
                              "ORDER BY date)b "
                              "ON a.id = b.id";
};

#endif // QUERYS_H
