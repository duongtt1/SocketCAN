import cantools
from jinja2 import Environment, FileSystemLoader, select_autoescape
import os
import json

# Load DBC file
db = cantools.database.load_file('./hyundai_kia_generic.dbc')

# Load configuration from JSON file
with open('./conf.json') as config_file:
    config = json.load(config_file)

# Filter CAN IDs based on configuration
filtered_messages = [msg for msg in db.messages if msg.frame_id in config['generate_can_ids']]

# Jinja environment setup
env = Environment(
    loader=FileSystemLoader(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates')),
    autoescape=select_autoescape(['c']),
    trim_blocks=True,
    lstrip_blocks=True
)

# Create Jinja template
template = env.get_template('./new_gen.jinja')

# Prepare context for template rendering
context = {
    'messages': filtered_messages,
}

# Render the template and write to a C file
output_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../Common/CNF_CLASS_CAN.h')
with open(output_path, 'w') as file:
    file.write(template.render(context))

print(f'C code generated and saved to {output_path}')