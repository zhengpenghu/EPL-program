/* main test simulator */
#include <iostream>
#include "CraigUtils.h"
#include "Window.h"
#include "tokens.h"
#include "ObjInfo.h"
#include "QuadTree.h" 
#include "Params.h"
#include "LifeForm.h"
#include "Event.h"

// zh3499 MAY 7TH 22:26

using namespace std;
extern int  sta;

template <typename T>
void bound(T& x, const T& min, const T& max) {
	assert(min < max);
	if (x > max) { x = max; }
	if (x < min) { x = min; }
}



ObjInfo LifeForm::info_about_them(SmartPointer<LifeForm> neighbor) {
	ObjInfo info;
     //cout<<"in info_about_them"<<endl;
	info.species = neighbor->species_name();
	info.health = neighbor->health();
	info.distance = pos.distance(neighbor->position());
	info.bearing = pos.bearing(neighbor->position());
//    cout<<"after bearing"<<endl;
	info.their_speed = neighbor->speed;
	info.their_course = neighbor->course;
	return info;
}



void LifeForm::update_position(void){
    if (!this->is_alive) {
        return;
    }
//   cout<<this->species_name();
//    cout<<"in_update_position"<<endl;

    SimTime delta_time = Event::now() - update_time;
    
    //cout<<id<<endl;
//    cout<<"update time"<<endl;
//    cout<<update_time<<endl;
//    cout<<delta_time<<endl;
    
    double energy_cost;
    
    if(delta_time>0.001){
        
        //position updated
        
        update_time = Event::now();
        Point temp {cos(course)*speed * delta_time,sin(course)*speed*delta_time};

        Point pos1;             //the new position it will move to
        pos1 = pos + temp;
    
  
        
        if(space.is_out_of_bounds(pos1)) {this->die();
            return;
        } //check if it's out of bound,if yes,kill it
    
        space.update_position(pos, pos1);
        pos = pos1;
        
        //energy consuming
        energy_cost = movement_cost(speed, delta_time);
        energy -= energy_cost;
        if (energy<min_energy) {
            this->die();
        }
       
    }
};

void LifeForm::border_cross(void){
    if (!this->is_alive) {
        return;
    }
//    cout<<this->species_name();
//    cout<<"in_border_cross"<<endl;
    update_position();
    if (!is_alive) {
        return;
    }
    //check for encounter if yes, resolve_encounter in the check_encounter function
    check_encounter();
    
    compute_next_move();
     //schedule the corss bounder event

}

void LifeForm::region_resize(void){
   //  cout<<"in_region_resize"<<endl;
    if (!this->is_alive) {
        return;
    }
    
    if (border_cross_event!=nullptr) {
        border_cross_event->cancel();
    }
   
    update_position();
    compute_next_move();
    
    
};

void LifeForm::set_course(double c){
//    cout<<this->species_name();
//     cout<<"in_set_course"<<endl;
//    
    if (!is_alive) {
        return;
    }
    SmartPointer<LifeForm> self = SmartPointer<LifeForm>(this);
    update_position();
    
    if (!is_alive) {
        return;
    }
    course = c;
    if(border_cross_event!=nullptr) border_cross_event->cancel();
//    if (!is_alive) {
//        return;
//    }
    
    compute_next_move();
};

void LifeForm::set_speed(double s){

    if (!is_alive) {
        return;
    }
    
    SmartPointer<LifeForm> self = SmartPointer<LifeForm>(this);
     update_position();
    if (!is_alive) {
        return;
    }
    
    speed =s;
    if(speed>max_speed) speed = max_speed;
    if(border_cross_event!=nullptr) border_cross_event->cancel();
    
    compute_next_move();
};

//Jeremy call age
void LifeForm::age(void){
    if (!is_alive) {
        return;
    }
    SmartPointer<LifeForm> self = SmartPointer<LifeForm>(this);
    
    energy-=age_penalty;
    
    
   // cout<<"energy "<<energy<<endl;
    if (energy<min_energy) {
              self->die();
        return;
    }
    new Event(age_frequency,[self](void) { self->age(); });
    };

void LifeForm::check_encounter(void){
    //use quadtree::findnearest and if yes, just call the encounter method of both two

    if (!is_alive) {
        return;
    }
//    cout<<this->species_name();
//    cout<<"check encounter"<<endl;
    //auto that = space.closest(this->pos);
    SmartPointer<LifeForm> that = space.closest(this->pos);
    
      that->update_position();
    if (!that->is_alive) {
        return;
    }
       double dis = this->pos.distance(that->pos);
  
    if (dis<1) {

        resolve_encounter(that);
    }
};

void LifeForm::resolve_encounter(SmartPointer<LifeForm>that){
    if (!this->is_alive) {
        return;
    }
//    cout<<this->species_name();
//    cout<<" resolve encounter"<<endl;
    SmartPointer<LifeForm> self = SmartPointer<LifeForm>(this);
    bool chance1 = false;
    bool chance2 = false;
//    cout<<"before here";
    energy-=encounter_penalty;
    that->energy-=encounter_penalty;
    if (energy<min_energy) {
        self->die();
    }
    if (that->energy<min_energy) {
        that->die();
    }
//    if ((!self->is_alive)&&(!that->is_alive)) {
//        return;
//    }
    
    if (!self->is_alive) {
                return;
            }
    if (!that->is_alive) {
        return;
    }
    
    ObjInfo selfinfo = that->info_about_them(self);
    ObjInfo thatinfo = info_about_them(that);
    
    auto self_decision =  self->encounter(thatinfo);
    auto that_decision =  that->encounter(selfinfo);
//    if(self_decision == Action::LIFEFORM_EAT) chance1 = ((drand48()))<eat_success_chance(self->energy, that->energy);
//    if(that_decision == Action::LIFEFORM_EAT) chance2 = ((drand48()))<eat_success_chance(that->energy, self->energy);

    if(self_decision == Action::LIFEFORM_EAT) chance1 = ((drand48()))<eat_success_chance(self->energy, thatinfo.health*start_energy);
    if(that_decision == Action::LIFEFORM_EAT) chance2 = ((drand48()))<eat_success_chance(thatinfo.health*start_energy, self->energy);

    if (chance1&&chance2) {
        switch (encounter_strategy) {
            case FASTER_GUY_WINS:
                if (self->get_speed()>that->get_speed()) {
                    self->eat(that);
                }
                else that->eat(self);
                break;
            case BIG_GUY_WINS:
                if (self-health()>that->health()) {
                    self->eat(that);
                }
                else that->eat(self);
                break;
            case UNDERDOG_IS_HERE:
                if (self-health()<that->health()) {
                    self->eat(that);
                }
                else that->eat(self);
                break;
            case SLOWER_GUY_WINS:
                if (self->get_speed()<that->get_speed()) {
                    self->eat(that);
                }
                else that->eat(self);
                break;
            case EVEN_MONEY:
                if (rand()%2) {
                    self->eat(that);
                }
                else that->eat(self);
                break;
//            default:
//                break;
        }
    }
    else if (chance1) self->eat(that);
    else if (chance2) that->eat(self);


}

void LifeForm::eat(SmartPointer<LifeForm> that){
    
//    cout<<this->species_name();
//    cout<<" eat"<<endl;
    
    if (!is_alive) {
        return;
    }
    SmartPointer<LifeForm> self = SmartPointer<LifeForm>(this);
    double that_energy = that->health()*start_energy;
    
    that->die();
    energy-=eat_cost_function(0.0,0.0);
    if (energy<min_energy) {
        self->die();
    }
    //new Event(digestion_time,[self,that_energy](){self->gain_energy(that_energy);});
    new Event(digestion_time,[self,that_energy](){self->energy +=that_energy*eat_efficiency;});

}


void LifeForm::gain_energy(double that_energy){
    energy = that_energy;
    //this->energy+=that_energy*eat_efficiency-eat_cost_function(this->energy,that_energy);
}

void LifeForm::compute_next_move(void){
    //when speed equals to zero, set the next border_cross event to be nullptr
    if ((!is_alive)) {
        return;
    }
    SmartPointer<LifeForm> self = SmartPointer<LifeForm>(this);
    if (speed>0) {
        //SimTime t = space.distance_to_edge(pos, course)/speed;
        SimTime t = space.distance_to_edge(pos, course)/speed+Point::tolerance/speed;
        border_cross_event = new Event(t, [self](void) { self->border_cross(); });
    }
    else border_cross_event = nullptr;
}

void LifeForm::reproduce(SmartPointer<LifeForm> that){
    if (!is_alive) return;
    
    if ((Event::now()-reproduce_time)< min_reproduce_time) {
        that->die();
        return;
    }
    
    update_position();
    if (!is_alive) {
        return;
    }
    reproduce_time = Event::now();
    //cout<<"in reproduce"<<endl;
    Point position;
    Point tem;
    double rrr;
    double tem_course;
    bool occupied = true;
    for (int i=0; (i<5)&&occupied; i++) {
        rrr =(reproduce_dist-encounter_distance)*drand48()+encounter_distance;
        tem_course = rand()/10.0;
//        tem.xpos =(reproduce_dist-encounter_distance)*drand48()+encounter_distance;
//        tem.ypos =(reproduce_dist-encounter_distance)*drand48()+encounter_distance;
        tem.xpos = rrr*cos(tem_course);
        tem.ypos = rrr*sin(tem_course);
        position = pos+ tem;
        if (space.is_out_of_bounds(position)) {// if the point is out of bound then try next point
            continue;
        }
        occupied = space.is_occupied(position);
     
    }
    if (occupied) {             //if no one is reproduce
        that->die();
        return;
    }
    if (!occupied) {    }
    //cout<<that->pos.xpos<<"   "<<that->pos.ypos<<endl;
    
    that->pos = position;
    
    that->is_alive = true;
    double energy_remaining = this->health()*start_energy/2*(1.0-reproduce_cost);
    energy = energy_remaining;
    that->energy = energy_remaining;
    //->gain_energy(energy_remaining);
    if (energy<min_energy) {
        this->die();
    }
    if (that->energy<min_energy) {
        that->die();
    }

    
    if(is_alive){
    space.insert(that, that->pos, [that]() { that->region_resize(); });
    (void) new Event(age_frequency, [that](void) { that->age(); });
    }
    //energy assignment
   
};

ObjList LifeForm::perceive(double rad){
    
    if (rad>100) rad=100;
    if (rad<2) rad=2;
//    int i=0;
//    assert(i==1);
//    
    update_position();
    
    
    SmartPointer<LifeForm> self = SmartPointer<LifeForm>(this);
//    cout<<this->species_name();
//    cout<<"in perceive"<<endl;
    std::vector<SmartPointer<LifeForm>> vector =  space.nearby(this->position(), rad);
    auto b = vector.begin();
    auto e = vector.end();
    ObjList info;// = vector
    while (b!=e) {
        auto in = this->info_about_them(*b);
        info.push_back(in);
        b++;
    }
    
    //energy cost
    energy-=perceive_cost(rad);
    if (energy<min_energy) {
        self->die();
    }
    return info;
};
















