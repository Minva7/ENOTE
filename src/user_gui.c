#include "../lib/user_gui.h"
#include "lvgl.h"
#include <Arduino.h>
//#include <lv_demos.h>



typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
} disp_size_t;

static disp_size_t disp_size;
lv_obj_t* switch_obj;
static lv_style_t style_title;
static lv_style_t style_cb_finsh;
lv_obj_t * current_time;
lv_obj_t * cb_1;
lv_obj_t * cb_2;
lv_obj_t * cb_3;
lv_obj_t * cb_4;
lv_obj_t * cb_5;
lv_obj_t * cb_6;

void user_button(unsigned char size_x,unsigned char size_y,lv_align_t site_t,unsigned char site_x,unsigned char site_y)
{
    switch_obj = lv_switch_create(lv_scr_act());
    lv_obj_set_size(switch_obj,size_x,size_y);
    lv_obj_align(switch_obj,site_t,site_x,site_y);//LV_ALIGN_CENTER

}

void user_button_logic(void)
{
    static unsigned char button_flag=0,button_flag_back=0;
    static unsigned char time_secend=0;
    char time_r[20]="2022-11-12 09:";
    button_flag = lv_obj_has_state(switch_obj, LV_STATE_CHECKED);

    if(button_flag!=button_flag_back)
    {
        button_flag_back=button_flag;
        if(button_flag==1)
        {
            sprintf(time_r,"2022-11-12 09:%02d",time_secend++);


            lv_obj_add_flag(current_time, LV_OBJ_FLAG_HIDDEN);


            current_time = lv_label_create(lv_scr_act());
            lv_obj_align(current_time,LV_ALIGN_TOP_MID,160,20);
            lv_label_set_text(current_time,time_r);
            lv_obj_add_style(current_time, &style_title, 0);

            printf("the switch is ON\r\n");
        }
        else
        {
            printf("the switch is OFF\r\n");
        }
    }
}

void user_cb1_logic(lv_event_t *cb)
{
    lv_event_code_t code_flag;
    code_flag=lv_event_get_code(cb);
    if(code_flag==LV_EVENT_VALUE_CHANGED)
    {
        
        if(lv_obj_has_state(cb_1, LV_STATE_CHECKED))
        {
            lv_obj_set_style_opa(cb_1,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_1,lv_color_hex(0x0fe0f0),LV_PART_MAIN);  //颜色
            lv_obj_invalidate(cb_1);
            printf("the cb_1 check on\r\n");
        }
        else
        {
            lv_obj_set_style_opa(cb_1,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_1,lv_color_hex(0xffffff),LV_PART_MAIN);
            printf("the cb_1 check off\r\n");
        }

    }
}

void user_cb2_logic(lv_event_t *cb)
{
    lv_event_code_t code_flag;
    code_flag=lv_event_get_code(cb);
    if(code_flag==LV_EVENT_VALUE_CHANGED)
    {
        
        if(lv_obj_has_state(cb_2, LV_STATE_CHECKED))
        {
            lv_obj_set_style_opa(cb_2,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_2,lv_color_hex(0x0fe0f0),LV_PART_MAIN);  //颜色
            lv_obj_invalidate(cb_2);
            printf("the cb_2 check on\r\n");
        }
        else
        {
            lv_obj_set_style_opa(cb_2,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_2,lv_color_hex(0xffffff),LV_PART_MAIN);
            printf("the cb_2 check off\r\n");
        }

    }
}

void user_cb3_logic(lv_event_t *cb)
{
    lv_event_code_t code_flag;
    code_flag=lv_event_get_code(cb);
    if(code_flag==LV_EVENT_VALUE_CHANGED)
    {        
        if(lv_obj_has_state(cb_3, LV_STATE_CHECKED))
        {
            lv_obj_set_style_opa(cb_3,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_3,lv_color_hex(0x0fe0f0),LV_PART_MAIN);  //颜色
            lv_obj_invalidate(cb_3);
            printf("the cb_3 check on\r\n");
        }
        else
        {
            lv_obj_set_style_opa(cb_3,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_3,lv_color_hex(0xffffff0),LV_PART_MAIN);
            printf("the cb_3 check off\r\n");
        }

    }
}

void user_cb4_logic(lv_event_t *cb)
{
    lv_event_code_t code_flag;
    code_flag=lv_event_get_code(cb);
    if(code_flag==LV_EVENT_VALUE_CHANGED)
    {        
        if(lv_obj_has_state(cb_4, LV_STATE_CHECKED))
        {
            lv_obj_set_style_opa(cb_4,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_4,lv_color_hex(0x0fe0f0),LV_PART_MAIN);  //颜色
            lv_obj_invalidate(cb_4);
            printf("the cb_4 check on\r\n");
        }
        else
        {
            lv_obj_set_style_opa(cb_4,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_4,lv_color_hex(0xffffff),LV_PART_MAIN);
            printf("the cb_4 check off\r\n");
        }
    }
}

void user_cb5_logic(lv_event_t *cb)
{
    lv_event_code_t code_flag;
    code_flag=lv_event_get_code(cb);
    if(code_flag==LV_EVENT_VALUE_CHANGED)
    {        
        if(lv_obj_has_state(cb_5, LV_STATE_CHECKED))
        {
            lv_obj_set_style_opa(cb_5,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_5,lv_color_hex(0x0fe0f0),LV_PART_MAIN);  //颜色
            lv_obj_invalidate(cb_5);
            printf("the cb_5 check on\r\n");
        }
        else
        {
            lv_obj_set_style_opa(cb_5,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_5,lv_color_hex(0xffffff),LV_PART_MAIN);
            printf("the cb_5 check off\r\n");
        }
    }
}

void user_cb6_logic(lv_event_t *cb)
{
    lv_event_code_t code_flag;
    code_flag=lv_event_get_code(cb);
    if(code_flag==LV_EVENT_VALUE_CHANGED)
    {        
        if(lv_obj_has_state(cb_6, LV_STATE_CHECKED))
        {
            lv_obj_set_style_opa(cb_6,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_6,lv_color_hex(0x0fe0f0),LV_PART_MAIN);  //颜色
            lv_obj_invalidate(cb_6);
            printf("the cb_6 check on\r\n");
        }
        else
        {
            lv_obj_set_style_opa(cb_6,LV_OPA_100,LV_PART_MAIN);  //透明度
            lv_obj_set_style_text_color(cb_6,lv_color_hex(0xffffff),LV_PART_MAIN);
            printf("the cb_6 check off\r\n");
        }
    }
}

lv_obj_t * notifications;
void user_xbox(unsigned char cb_amount)
{
    lv_obj_t * title = lv_label_create(lv_scr_act());
    static lv_style_t style_cb1;

    static lv_style_t style; 
    lv_obj_t* obj = lv_obj_create(lv_scr_act());
    lv_style_init(&style);
    lv_style_set_radius(&style, 0);
    /*Make a gradient*/
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_NONE, 1));
    lv_style_set_border_width(&style,0);
   // lv_style_set_bg_grad_color(&style, lv_palette_main(LV_PALETTE_BLUE));
   // lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);
  //  lv_style_set_bg_main_stop(&style, 1);
   // lv_style_set_bg_grad_stop(&style, 149);
    /*Create an object with the new style*/
    
    //lv_obj_set_style_size(obj,480,2);
    lv_obj_set_width(obj, 480);
    lv_obj_set_height(obj, 480);
    lv_obj_set_x(obj,0);
    lv_obj_set_y(obj,0);
    lv_obj_set_height(obj, LV_PCT(500));
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(obj, 1); 
    lv_obj_add_style(obj, &style, 0);
   // lv_obj_align(obj,LV_ALIGN_OUT_TOP_LEFT,0,0);
   // lv_palette_main(LV_PALETTE_GREEN);


   //  windows_bg = lv_obj_create(lv_scr_act());//创建对象
    
 //   notifications = lv_obj_create(lv_scr_act());
  //  lv_obj_t * current_time = lv_label_create(lv_scr_act());
    
/*
    if(disp_size == DISP_SMALL) {
        lv_obj_add_flag(notifications, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_set_height(notifications, LV_PCT(200));
    }
    else  {
        lv_obj_set_height(notifications, LV_PCT(200));
        lv_obj_set_style_max_height(notifications, 300, 50);
    }
*/
/*
    lv_obj_set_height(notifications, LV_PCT(500));
    lv_obj_set_style_width(notifications, 500, 500);
    lv_obj_set_size(notifications,480,500);
    lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_GREY, 1));


    //lv_obj_set_style_bg_color(notifications,lv_palette_lighten(LV_PALETTE_RED,50),1);
    //lv_style_set_bg_color(notifications,lv_palette_lighten(LV_PALETTE_RED,1)); 
    lv_obj_set_style_bg_color(notifications, lv_palette_lighten(LV_PALETTE_RED,255), 255);
    //lv_style_set_bg_color(windows_bg,lv_palette_lighten(LV_PALETTE_BLUE,50));
    
    lv_obj_set_flex_flow(notifications, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(notifications, 1);                     
*/
    title = lv_label_create(obj);
    lv_label_set_recolor(title, 1);
    lv_label_set_text(title, "#ff802a Agent");
 // lv_obj_align(title,LV_ALIGN_TOP_LEFT,20,0);

  //  lv_obj_add_style(title, &style_title, 0);
   // LV_PART_MAIN
/*
    current_time = lv_label_create(lv_scr_act());
    lv_obj_align(current_time,LV_ALIGN_TOP_MID,160,20);
    lv_label_set_text(current_time, "2022-11-12 09:03");
    lv_obj_add_style(current_time, &style_title, 0);
*/
    if(cb_amount > 0)
        cb_1 = lv_checkbox_create(obj);
    else ;
    if(cb_amount > 1)
    cb_2 = lv_checkbox_create(obj);
    else ;
    if(cb_amount > 2)
    cb_3 = lv_checkbox_create(obj);
    else ;
    if(cb_amount > 3)
    cb_4 = lv_checkbox_create(obj);
    else ;
    if(cb_amount > 4)
    cb_5 = lv_checkbox_create(obj);
    else ;
    if(cb_amount > 5)
    cb_6 = lv_checkbox_create(obj);
    else ;


/*
    cb_1 = lv_checkbox_create(notifications);
    //lv_label_set_recolor(cb_1, 1);

    lv_checkbox_set_text(cb_1, buf);
    lv_obj_add_style(cb_1, &style_cb_finsh, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(cb_1, user_cb1_logic, LV_EVENT_VALUE_CHANGED, 2);



    cb_2 = lv_checkbox_create(notifications);
    lv_checkbox_set_text(cb_2, "New connection");
    lv_obj_add_event_cb(cb_2, user_cb2_logic, LV_EVENT_VALUE_CHANGED, 2);

    cb_3 = lv_checkbox_create(notifications);
    lv_checkbox_set_text(cb_3, "New subscriber");
  //  lv_obj_add_state(cb, LV_STATE_CHECKED);

    cb_4 = lv_checkbox_create(notifications);
    lv_checkbox_set_text(cb_4, "New message");
  //  lv_obj_add_state(cb, LV_STATE_DISABLED);

    cb_5 = lv_checkbox_create(notifications);
    lv_checkbox_set_text(cb_5, "Milestone reached");
  //  lv_obj_add_state(cb, LV_STATE_CHECKED | LV_STATE_DISABLED);

    cb_6 = lv_checkbox_create(notifications);
    lv_checkbox_set_text(cb_6, "Out of stock");
    */
}

void user_gui_cb(unsigned char cb_num, char *buf)
{
    
    switch (cb_num)
    {
        case 1: 
                
                //lv_label_set_recolor(cb_1, 1);
                lv_checkbox_set_text(cb_1, buf);
                lv_obj_set_style_text_color(cb_1,lv_color_hex(0xffffff),LV_PART_MAIN);
                lv_obj_add_style(cb_1, &style_cb_finsh, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(cb_1, user_cb1_logic, LV_EVENT_VALUE_CHANGED, 2);
                break;

        case 2:
               // cb_2 = lv_checkbox_create(notifications);
                //lv_label_set_recolor(cb_1, 1);
                lv_checkbox_set_text(cb_2, buf);
                lv_obj_set_style_text_color(cb_2,lv_color_hex(0xffffff),LV_PART_MAIN);
                lv_obj_add_style(cb_2, &style_cb_finsh, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(cb_2, user_cb2_logic, LV_EVENT_VALUE_CHANGED, 2);
                break;
        case 3:
               // cb_3 = lv_checkbox_create(notifications);
                //lv_label_set_recolor(cb_1, 1);
                lv_checkbox_set_text(cb_3, buf);
                lv_obj_set_style_text_color(cb_3,lv_color_hex(0xffffff),LV_PART_MAIN);
                lv_obj_add_style(cb_3, &style_cb_finsh, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(cb_3, user_cb3_logic, LV_EVENT_VALUE_CHANGED, 2);
                break;
        case 4:
              //  cb_4 = lv_checkbox_create(notifications);
                //lv_label_set_recolor(cb_1, 1);
                lv_checkbox_set_text(cb_4, buf);
                lv_obj_set_style_text_color(cb_4,lv_color_hex(0xffffff),LV_PART_MAIN);
                lv_obj_add_style(cb_4, &style_cb_finsh, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(cb_4, user_cb4_logic, LV_EVENT_VALUE_CHANGED, 2);
                break;

        case 5:
             //   cb_5 = lv_checkbox_create(notifications);
                //lv_label_set_recolor(cb_1, 1);
                lv_checkbox_set_text(cb_5, buf);
                lv_obj_set_style_text_color(cb_5,lv_color_hex(0xffffff),LV_PART_MAIN);
                lv_obj_add_style(cb_5, &style_cb_finsh, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(cb_5, user_cb5_logic, LV_EVENT_VALUE_CHANGED, 2);
                break;
        case 6:
              //  cb_6 = lv_checkbox_create(notifications);
                //lv_label_set_recolor(cb_1, 1);
                lv_checkbox_set_text(cb_6, buf);
                lv_obj_set_style_text_color(cb_6,lv_color_hex(0xffffff),LV_PART_MAIN);
                lv_obj_add_style(cb_6, &style_cb_finsh, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(cb_6, user_cb6_logic, LV_EVENT_VALUE_CHANGED, 2);
                break;
        default: break;
    }
}

void user_gui(unsigned char cb_amount)
{
 //   user_button(80,40,LV_ALIGN_CENTER,400,400);
    user_xbox(cb_amount);
}

void user_lg(void)
{
 //   user_button_logic();
}
