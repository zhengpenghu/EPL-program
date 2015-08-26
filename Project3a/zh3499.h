//
//  zh3499.h
//  epl
//
//  Created by zhengpeng hu on 15/5/4.
//  Copyright (c) 2015年 zhengpeng hu. All rights reserved.
//
#include <memory>
#include "LifeForm.h"
#include "Init.h"

#ifndef epl_zh3499_h
#define epl_zh3499_h
class zh3499 : public LifeForm {
    
    //friend class zh3499;
protected:
    static void initialize(void);
    void spawn(void);
    void hunt(void);
    void startup(void);
    Event* hunt_event;
    bool onmyway(ObjList);
    void change_course(void);
    int id;
    double back_distance;
    double back_course;
    double last_time;
    void get_back();
    void catchyou(ObjInfo);
//    int decide;
   bool course_changed ;
    std::string name="zh3499";
    
    //Point my_position;
public:
    zh3499(void);
    ~zh3499(void);
    Color my_color(void) const;   // defines LifeForm::my_color
    static SmartPointer<LifeForm> create(void);
    virtual std::string species_name(void) const;
    std::string player_name(void) const;
  
    virtual Action encounter(const ObjInfo&);
    friend class Initializer<zh3499>;
};


#endif
