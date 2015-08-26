//
//  zh3499.cpp
//  epl
//
//  Created by zhengpeng hu on 15/5/4.
//  Copyright (c) 2015年 zhengpeng hu. All rights reserved.
//

// udpated at May 10th
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "zh3499.h"
#include "CraigUtils.h"
#include "Event.h"
#include "ObjInfo.h"
#include "Params.h"
#include "Random.h"
#include "Window.h"

#ifdef _MSC_VER
using namespace epl;
#endif
using namespace std;
using String = std::string;

Initializer<zh3499> __zh3499_initializer;

String zh3499::species_name(void) const
{
    return name;
}

String zh3499::player_name(void) const
{
    return "zh3499";
}

Action zh3499::encounter(const ObjInfo& info)
{
     SmartPointer<zh3499> me{this};
    if ((info.species == name)&&(info.species.substr(0,6) =="zh3499")){
 
        hunt_event->cancel();
        //name=info.species;
        new Event(0.1,[me](void){me->name ="zh3499:"+std::to_string(me->id);});
        set_course(-info.bearing);
        hunt_event = new Event(0.0, [me] (void) { me->hunt(); });
        //get_back();
        return LIFEFORM_EAT;
        }
    else if(info.species.substr(0,6) =="zh3499"){
        get_back();

        set_course(info.bearing + M_PI);
//        hunt_event->cancel();
//        hunt_event = new Event(1,[me] (void) { me->hunt();});
        return LIFEFORM_IGNORE;
    }
    else if(info.species=="Algae"){
        hunt_event->cancel();
        set_course(-info.bearing);
        hunt_event = new Event(0.0, [me] (void) { me->hunt(); });
        return LIFEFORM_EAT;
        }
  else{
            name=info.species;
            new Event(0.1,[me](void){me->name ="zh3499:"+std::to_string(me->id);});
            hunt_event->cancel();
           
            set_course(-info.bearing);
            hunt_event = new Event(0.0, [me] (void) { me->hunt(); });
            //get_back();
            return LIFEFORM_EAT;
        }
    }


//Action zh3499::encounter(const ObjInfo& info)
//{
//    if (info.species == species_name()) {
//        /* don't be cannibalistic */
//        set_course(info.bearing + M_PI);
//        return LIFEFORM_IGNORE;
//    }
//    else {
//        hunt_event->cancel();
//        SmartPointer<zh3499> me{this};
//        hunt_event = new Event(0.0, [me] (void) { me->hunt(); });
//        return LIFEFORM_EAT;
//    }
//}
//
void zh3499::initialize(void) {
    LifeForm::add_creator(zh3499::create, "zh3499");
}

/*
 * REMEMBER: do not call set_course, set_speed, perceive, or reproduce
 * from inside the constructor!!!!
 * you must wait until the object is actually alive
 */
zh3499::zh3499() {
    SmartPointer<zh3499> self = SmartPointer<zh3499>(this);
    new Event(0, [self](void) { self->startup(); });
    id = rand();
    name = "zh3499:"+std::to_string(id);
    //last_time= Event::now();
//    back_course=0;
//    back_distance=0;
}

zh3499::~zh3499() {}

void zh3499::startup(void) {
    last_time= Event::now();
    back_distance=0;
    back_course=0;
    set_course(drand48() * 2.0 * M_PI);
    set_speed(2 + 5.0 * drand48());
    SmartPointer<zh3499> self = SmartPointer<zh3499>(this);
    hunt_event = new Event(0, [self](void) { self->hunt(); });
}

void zh3499::spawn(void) {
    SmartPointer<zh3499> child = new zh3499;
    reproduce(child);
    //child->my_position= child->coun;// save the start position
}


Color zh3499::my_color(void) const {
    return MAGENTA;
}

SmartPointer<LifeForm> zh3499::create(void) {
    return new zh3499;
}


void zh3499::hunt(void) {
    const String fav_food = "Algae";
     SmartPointer<zh3499> me{this};
    ObjList prey;
    hunt_event = Nil<Event>();
    if (health() == 0) { return; }
    
    get_back();
    if (Event::now()<20) {
        prey = perceive(40.0);
    }
    else
    prey = perceive(40.0);
    
    double best_d = HUGE;
    
    //if I am strong then go out to eat
    if (health()>6) {
        for (ObjInfo i : prey){
            if(i.species.substr(0,6)!="zh3499"){
                if ((i.health<(health()-2))&&(i.health>0.5)) {
              //      course_changed = 0 ;
                    //if ((cos(M_PI+i.bearing-i.their_course)>0.72)&&(best_d > i.distance) ){
                    if ((best_d > i.distance) ){
//                        set_course(i.bearing);
//                        set_speed(7);
                        catchyou(i);
                        
                        best_d = i.distance;
                    }
                }
            }
        }
    }
 
        //Algae is my favourite food
    for (ObjInfo i : prey) {
        //count++ ;
        if (i.species == fav_food) {
           // course_changed = 0 ;
            if (best_d > i.distance) {
                set_course(i.bearing);
                if (i.distance>30) {
                    set_speed(8);
                }
                else
                set_speed(10);
                best_d = i.distance;
            }
        }
    }
    
    //if I can find any food, then try to find is there anyone on my way, otherwise I might have reach the bound of the space
    if(best_d == HUGE){
        //if(course_changed == false){
            //course_changed = 0 ;
            if (!onmyway(prey)) {
                change_course();
            }
   //     }
        //course_changed^=1;
    }
    
   
    hunt_event = new Event(6.0, [me] (void) { me->hunt();});
    if ((health() >= 8.0)&&(back_distance<100)) spawn();
}

bool zh3499::onmyway(ObjList info){
    if (info.capacity()==0) {
        return false;
    }
    for (ObjInfo i: info) {
        if (i.species.substr(0,6)!="zh3499")         //cout<<"cos"<<cos(i.bearing-get_course())<<endl;
        if ((cos(i.bearing-get_course())>0.85)) {
            //cout<<"cos"<<cos(i.bearing-get_course())<<endl;
            return true;
        }
    }
    return false;
}

void zh3499::catchyou(ObjInfo that){
    double tem= that.their_speed/10*sin(that.bearing-that.their_course);
    set_speed(10);
    set_course(that.bearing-asin(tem));
}

void zh3499::change_course(void){
    //cout<<"in change course"<<endl;
    switch (rand()%8) {
        case 0:
            set_speed(8);
            break;
        case 3:
            set_speed(3);
            break;
        case 4:
            set_speed(4);
            break;
        case 5:
            set_speed(5);
            break;
        case 6:
            set_speed(6);
            break;
        case 7:
            set_speed(7);
            break;
        default:
            set_speed(7);
            break;
    }
    set_course(M_PI+back_course);

}

//every time change speed or change course call this function
void zh3499::get_back(void){
    double this_distance= (Event::now()-last_time)*get_speed();
    double tem_x = (back_distance*cos(back_course)+this_distance*cos(get_course()));
    double tem_y = (back_distance*sin(back_course)+this_distance*sin(get_course()));
    back_course = atan2(tem_y, tem_x);
    back_distance = sqrt(tem_x*tem_x+tem_y*tem_y);
    last_time = Event::now();
}
