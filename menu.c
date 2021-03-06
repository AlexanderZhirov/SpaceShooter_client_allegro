#include "common.h"
#include "menu.h"

/************************************************************
 * HANDLING
 ***********************************************************/

void menu_handling(CLIENT_DATA* data)
{
    if(data->event.current.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch (data->event.current.keyboard.keycode)
        {
            case ALLEGRO_KEY_ESCAPE:
            {
                if(!data->keys[ESCAPE])
                {
                    data->keys[ESCAPE] = true;
                    data->done = true;                
                }
                break;
            }
            case ALLEGRO_KEY_LEFT:
                data->keys[LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                data->keys[RIGHT] = true;
                break;
            case ALLEGRO_KEY_UP:
                data->keys[UP] = true;
                cl_select_button(data);
                break;
            case ALLEGRO_KEY_DOWN:
                data->keys[DOWN] = true;
                cl_select_button(data);
                break;
            case ALLEGRO_KEY_ENTER:
                data->keys[ENTER] = true;
                menu_push_button(data);
                break;
            case ALLEGRO_KEY_SPACE:
                data->keys[SPACE] = true;
                menu_push_button(data);
                break;
        }
    }
    else if(data->event.current.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch (data->event.current.keyboard.keycode)
        {
            case ALLEGRO_KEY_ESCAPE:
                data->keys[ESCAPE] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                data->keys[LEFT] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                data->keys[RIGHT] = false;
                break;
            case ALLEGRO_KEY_UP:
                data->keys[UP] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                data->keys[DOWN] = false;
                break;
            case ALLEGRO_KEY_ENTER:
                data->keys[ENTER] = false;
                break;
            case ALLEGRO_KEY_SPACE:
                data->keys[SPACE] = false;
                break;
        }
    }
    else if(data->event.current.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        cl_update_button(data);
    }
    else if(data->event.current.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        menu_push_button(data);
    }
    else if(data->event.current.type == ALLEGRO_EVENT_TIMER)
    {
        data->render = true;
        
        cl_update_background(data, BG1);
        cl_update_background(data, BG2);
    }
}

/************************************************************
 * DRAW
 ***********************************************************/

void menu_draw(CLIENT_DATA* data)
{
    cl_draw_background(data, BG1);
    cl_draw_background(data, BG2);
    cl_draw_header(data);
    cl_draw_button(data);
}

/************************************************************
 * PROCESSING
 ***********************************************************/

void menu_init_button(CLIENT_DATA* data, int key_button, int key_font, const char* text)
{
    if(data->pairing.interface[MENU]->button_size > key_button)
    {
        CLIENT_FONT* font = data->media.fonts[key_font];
        int block_size = data->screen.height / 8;
        
        cl_add_interface_button(
                data->pairing.interface[MENU]->button[key_button],
                font->al_font, text, data->screen.width / 2,
                (block_size * (key_button + 2)) + (block_size / 4) + (block_size / 2) - font->size,
                (data->screen.width / 2) - (750 * data->screen.scale_factor) / 2,
                block_size * (key_button + 2) + (block_size / 4),
                (data->screen.width / 2) + (750 * data->screen.scale_factor) / 2,
                block_size * (key_button + 2) + block_size);
    }
}

static void menu_push_button(CLIENT_DATA* data)
{
    CLIENT_INTERFACE* interface = data->pairing.interface[data->state];

    for(int i = 0; i < interface->button_size; i++)
        if(interface->button[i]->selected)
        {
            cl_play_sound(data, SAMPLE_ENTER);
            
            switch(i)
            {
                case BUTTON_MENU_SINGLE:
                {
                    interface->button[i]->selected = false;
                    al_hide_mouse_cursor(data->screen.display);
                    cl_change_state(data, SINGLE);
                    break;
                }
                case BUTTON_MENU_NETWORK:
                {
                    interface->button[i]->selected = false;
                    cl_change_state(data, NETWORKMODE);
                    break;
                }
                case BUTTON_MENU_SETTINGS:
                    break;
                case BUTTON_MENU_AUTORS:
                {
                    interface->button[i]->selected = false;
                    cl_change_state(data, AUTORS);
                    break;
                }
                case BUTTON_MENU_OUT:
                    data->done = true;
                    break;
            }
        }
}
