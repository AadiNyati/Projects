import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk

class Tamagotchi:
    def __init__(self, name):
        self.name = name
        self.hunger = 5
        self.happiness = 5
        self.energy = 5
        self.alive = True

    def feed(self):
        if self.hunger < 10:
            self.hunger = min(10, self.hunger + 2)

    def play(self):
        if self.energy > 0:
            self.happiness = min(10, self.happiness + 2)
            self.energy = max(0, self.energy - 1)

    def sleep(self):
        self.energy = 10

    def tick(self):
        self.hunger -= 1
        self.happiness -= 1
        self.energy -= 1
        self.hunger = max(0, min(10, self.hunger))
        self.happiness = max(0, min(10, self.happiness))
        self.energy = max(0, min(10, self.energy))
        if self.hunger == 0 or self.happiness == 0 or self.energy == 0:
            self.alive = False

def update_status():
    if pet.alive:
        pet.tick()
        update_bars()
        update_pet_mood()
        root.after(6000, update_status)
    else:
        status_label.config(text=f"Oh no! {pet.name} has passed away!")
        pet_label.config(image=pet_images["dead"])
        pet_label.image = pet_images["dead"]
        feed_btn.config(state="disabled")
        play_btn.config(state="disabled")
        sleep_btn.config(state="disabled")

def update_pet_mood():
    if not pet.alive:
        pet_label.config(image=pet_images["dead"])
        pet_label.image = pet_images["dead"]
        return
    if pet.happiness >= 7 and pet.hunger >= 5 and pet.energy >= 5:
        mood = "happy"
    elif pet.happiness >= 4 and pet.hunger >= 3 and pet.energy >= 3:
        mood = "neutral"
    elif pet.energy <= 3:
        mood = "sleepy"
    else:
        mood = "sad"
    pet_label.config(image=pet_images[mood])
    pet_label.image = pet_images[mood]

def update_bars():
    hunger_label.config(text=f"Hunger: {pet.hunger}/10")
    happiness_label.config(text=f"Happiness: {pet.happiness}/10")
    energy_label.config(text=f"Energy: {pet.energy}/10")
    hunger_bar['value'] = pet.hunger
    happiness_bar['value'] = pet.happiness
    energy_bar['value'] = pet.energy

def feed_pet():
    pet.feed()
    update_bars()
    update_pet_mood()

def play_pet():
    pet.play()
    update_bars()
    update_pet_mood()

def sleep_pet():
    pet.sleep()
    update_bars()
    update_pet_mood()

def load_images():
    moods = ["happy", "neutral", "sad", "sleepy", "dead"]
    images = {}
    for mood in moods:
        img = Image.open(f"images/{mood}.png").resize((128, 128))
        images[mood] = ImageTk.PhotoImage(img)
    return images

name = input("Give your Tamagotchi a name: ")
pet = Tamagotchi(name)

root = tk.Tk()
root.title("Tamagotchi Game")

pet_images = load_images()

status_label = tk.Label(root, text=f"Taking care of {pet.name}!", font=("Arial", 14))
status_label.pack(pady=10)

pet_label = tk.Label(root, image=pet_images["happy"])
pet_label.image = pet_images["happy"]
pet_label.pack(pady=10)

hunger_label = tk.Label(root, text=f"Hunger: {pet.hunger}/10", font=("Arial", 12))
hunger_label.pack()
hunger_bar = ttk.Progressbar(root, length=200, maximum=10)
hunger_bar['value'] = pet.hunger
hunger_bar.pack(pady=5)

happiness_label = tk.Label(root, text=f"Happiness: {pet.happiness}/10", font=("Arial", 12))
happiness_label.pack()
happiness_bar = ttk.Progressbar(root, length=200, maximum=10)
happiness_bar['value'] = pet.happiness
happiness_bar.pack(pady=5)

energy_label = tk.Label(root, text=f"Energy: {pet.energy}/10", font=("Arial", 12))
energy_label.pack()
energy_bar = ttk.Progressbar(root, length=200, maximum=10)
energy_bar['value'] = pet.energy
energy_bar.pack(pady=5)

feed_btn = tk.Button(root, text="Feed", command=feed_pet)
feed_btn.pack(pady=5)

play_btn = tk.Button(root, text="Play", command=play_pet)
play_btn.pack(pady=5)

sleep_btn = tk.Button(root, text="Sleep", command=sleep_pet)
sleep_btn.pack(pady=5)

update_status()

root.mainloop()
