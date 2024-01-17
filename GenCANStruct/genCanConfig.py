import cantools
from jinja2 import Environment, FileSystemLoader, select_autoescape
import os
import json

generate_can_ids= [64, 66, 67, 68, 1154]

# Load DBC file
db = cantools.database.load_file('./hyundai_kia_generic.dbc')


# Filter CAN IDs based on configuration
filtered_messages = [msg for msg in db.messages if msg.frame_id in generate_can_ids]

# Jinja environment setup
env = Environment(
    loader=FileSystemLoader(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates')),
    autoescape=select_autoescape(['c']),
    trim_blocks=True,
    lstrip_blocks=True
)

# Create Jinja template
template = env.get_template('./define_can_template.jinja')

# Prepare context for template rendering
context = {
    'messages': filtered_messages,
}

# Render the template and write to a C file
output_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../Common/CNF_CAN.h')
with open(output_path, 'w') as file:
    file.write(template.render(context))

print(f'C code generated and saved to {output_path}')
