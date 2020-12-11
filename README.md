# LED-Streammarker for my Twitch setup

## Introduction
This is my idea of a visual feedback if someone sets a marker in my livestreams on twitch, to make it easier to cut a short summary video for youtube.
If an user do so, the streammarker starts fading x times.
I'm using a Wemos D1 mini + 35 WS2812b LEDs.

## Usage
Just flash the code onto an ESP-board, after you have edited the wifi-credentials.
And connect the LED-Stripe to Pin "D2".
After boot you are able to do a HTTP-Request:

`http://IP_OF_YOUR_DEVICE/signal?r=128&g=0&b=255&s=3&t=2`

### Parameters
* r(0-255) - red tone of your leds
* g(0-255) - green tone of your leds 
* b(0-255) - blue tone of your leds
* t - number of repeats of the animation
* s - number of speed of the animation

## Enclosure  
If you want to print the case I've used please take a look into the "stls"-folder.  
The files are named as what they are used for.
To get a better understanding, please watch the images, stored in the "images"-folder.

## Assembly
To assemble the 3d printed parts. You will need some glue, to glue the inlay parts.
Because I'm not very good in get parts in correct position, I've printed some helperparts.
Just place these parts onto the white inlay and glue the black inlays to the correct positions. After the glue dried, remove the helpers.

## Twitch

If you are interested in what I'm doing, just have a look at twitch:
[https://www.twitch.tv/chrisfigge](https://www.twitch.tv/chrisfigge)
