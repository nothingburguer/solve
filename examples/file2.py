import random

names = ["Arthos", "Ibrazen", "Borin", "Selene", "Kael"]
classes = ["Mercenary", "Ransom Broker", "Hired Blade", "Watchman"]
cities = ["Curaw", "Praven", "Saren"]

def generate_npc():
    npc_name = random.choice(names)
    npc_class = random.choice(classes)
    npc_city = random.choice(cities)
    npc_lvl = random.randint(1, 20)

    print(f"{npc_name}, {npc_class} - level {npc_lvl} from {npc_city}")

for i in range(5):
    generate_npc()

