#!/bin/bash

#A script to create a video out of a series of images using ffmpeg
#Written by juniorxsound <contact@orfleisher.com?

ffmpeg -r 25 -f frame_0 -s 800x800 -i frame_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p render.mp4