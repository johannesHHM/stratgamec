#!/usr/bin/env python3

import logging as log
import itertools

class UnitAnimationsData:
    def __init__(self, hero_id, unit_id, unit_name, states):
        self.hero_id = hero_id
        self.unit_id = unit_id
        self.unit_name = unit_name
        self.states = states
    
    def print(self):
        print(f"UnitAnimationsData\n unit_id: {self.unit_id}\n unit_name: {self.unit_name}\n states: {self.states}")
    
    def parse_animation_data(self):
        try:
            animation_file = open(f"../data/hero/{self.hero_id}/animations/{self.unit_name}/animationData", "r")
        except:
            log.error("Failed to open file: " + f"../data/hero/{self.hero_id}/animations/animationsData")

        line = animation_file.readline().strip()

        


class HeroAnimationData:
    def __init__(self, hero_id):
        self.hero_id = hero_id
        self.unit_count = 0
        self.unit_data_list = []
    
    def parse_animations_data(self):
        try:
            animations_file = open(f"../data/hero/{self.hero_id}/animations/animationsData", "r")
        except:
            log.error("Failed to open file: " + f"../data/hero/{self.hero_id}/animations/animationsData")
        
        line = animations_file.readline().strip()
        
        if line.isdigit():
            self.unit_count = int(line)
        else:
            log.error("Failed to read unitCount from animationsData, given line is: " + line)
        
        for _ in itertools.repeat(None, self.unit_count):
            line = animations_file.readline().strip()
            fields = line.split(",")
            if fields[0].isdigit():
                unit_id = int(fields[0])
            else:
                log.error("Failed to read unitType from animationsData, given line is: " + line)
            if fields[2].isdigit():
                states = int(fields[2])
            else:
                log.error("Failed to read unit state count from animationsData, given line is: " + line)
            unit_name = fields[1]

            self.unit_data_list.append(UnitAnimationsData(self.hero_id, unit_id, unit_name, states))
        
        for unit_anim_data in self.unit_data_list:
            unit_anim_data.print()

            unit_anim_data.parse_animation_data()
        

hero = HeroAnimationData(0)
hero.parse_animations_data()
