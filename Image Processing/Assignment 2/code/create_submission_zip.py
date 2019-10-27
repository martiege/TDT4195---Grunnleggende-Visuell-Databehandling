import os
import zipfile

def query(question, options):
    print(question)
    to_write = ["{}: {}".format(key, val) for key, val in options.items()]
    to_write = ", ".join(to_write)
    print("Options to select: " + to_write)
    answer = None
    while answer not in ("yes", "no"):
        answer_alternatives = ", ".join([str(key) for key in options.keys()])
        answer = input("Select an option [{}]:".format(answer_alternatives))
        answer = answer.strip()
        if answer not in options.keys():
            print("Answer is not in: {}".format(list(options.keys())))
            continue
        return options[answer]


files_to_include = [
    "dataloaders.py",
    "task2a.py",
    "task4a.py",
    "task4b.py",
    "task4c.py",
    "task4d.py",
    "trainer.py",
    "utils.py",
]

print("For task 4, select either the jupyter notebook or the python (.py) file.")
filepath = query("Which file should be included in the zip?",
                 {
                     "1": "task2b.py",
                     "2": "task2b.ipynb"
                 })
files_to_include.append(filepath)
filepath = query("Which file should be included in the zip?",
                 {
                     "1": "task2d.py",
                     "2": "task2d.ipynb"
                 })
files_to_include.append(filepath)

for filepath in files_to_include:
    if not os.path.isfile(filepath):
        print("Did not find file: {}".format(filepath))
        print("Terminating program without creating zipfile.")
        exit(0)

zipfile_path = "assignment_code.zip"


print("-"*80)
with zipfile.ZipFile(zipfile_path, "w") as fp:
    for filepath in files_to_include:
        fp.write(filepath)
        print("Adding file:", filepath)
print("-"*80)
print("Zipfile saved to: {}".format(zipfile_path))
print("Please, upload your assignment PDF file outside the zipfile to blackboard.")