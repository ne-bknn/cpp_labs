import json
import random
from string import ascii_letters, digits

def get_word():
    return "".join(random.choices(ascii_letters, k=random.randrange(3, 10))).lower().capitalize()

def gen_pub():
    pub = {}
    pub["type"] = random.choice(["fiction", "study", "scientific"])
    pub["author"] = get_word() + " " + get_word()
    pub["title"] = get_word()
    pub["code"] = get_word()
    pub["amount"] = random.randrange(3, 100)
    pub["publisher"] = get_word()
    pub["year"] = random.randrange(1500, 2022)

    if pub["type"] == "fiction":
        pub["genre"] = get_word()

    if pub["type"] == "scientific":
        pub["courses"] = [get_word() for _ in range(random.randrange(0, 4))]

    if pub["type"] == "study":
        pub["courses"] = [get_word(), ]
        pub["group_indecies"] = ["B19"+str(random.randrange(100, 500)) for _ in range(random.randrange(10))]

    return pub

def main():
    library = [gen_pub() for _ in range(30)]
    print(json.dumps(library))


if __name__ == "__main__":
    main()
