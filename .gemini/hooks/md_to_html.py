import sys, json, os, re

def render_table(lines):
    if len(lines) < 2: return '\n'.join(lines)
    
    # Filter out the separator line (|---|)
    rows = []
    for line in lines:
        if re.match(r'^\|[\s:-|]+\|$', line.strip()):
            continue
        cells = [c.strip() for c in line.strip('|').split('|')]
        rows.append(cells)
    
    if not rows: return ""
    
    html = '<table>\n<thead>\n<tr>'
    for cell in rows[0]:
        html += f'<th>{cell}</th>'
    html += '</tr>\n</thead>\n<tbody>\n'
    
    for row in rows[1:]:
        html += '<tr>'
        for cell in row:
            html += f'<td>{cell}</td>'
        html += '</tr>\n'
    
    html += '</tbody>\n</table>'
    return html

def process_markdown_body(text):
    # Diagrams/Images
    text = re.sub(r'!\[(.*?)\]\((.*?)\)', r'<div class="diagram"><img src="\2" alt="\1"></div>', text)
    
    # Note boxes (Blockquotes starting with Note: or Important:)
    text = re.sub(r'^> (Note:.*?|Important:.*?)\n((?:> .*\n?)*)', 
                  lambda m: f'<div class="note-box"><strong>{m.group(1)}</strong><br/>{m.group(2).replace("> ", "<br/>")}</div>', 
                  text, flags=re.MULTILINE)
    
    # Simple Blockquotes
    text = re.sub(r'^> (.*)', r'<blockquote>\1</blockquote>', text, flags=re.MULTILINE)
    
    # Tables
    lines = text.split('\n')
    new_lines = []
    in_table = False
    table_lines = []
    
    for line in lines:
        if line.strip().startswith('|') and '|' in line[1:]:
            if not in_table:
                in_table = True
            table_lines.append(line)
        else:
            if in_table:
                new_lines.append(render_table(table_lines))
                table_lines = []
                in_table = False
            new_lines.append(line)
    if in_table:
        new_lines.append(render_table(table_lines))
        
    text = '\n'.join(new_lines)
    
    # Bold / Italic
    text = re.sub(r'\*\*(.*?)\*\*', r'<strong>\1</strong>', text)
    text = re.sub(r'\*(.*?)\*', r'<em>\1</em>', text)
    
    # Code blocks
    text = re.sub(r'```(.*?)\n(.*?)```', r'<pre><code>\2</code></pre>', text, flags=re.DOTALL)
    text = re.sub(r'`(.*?)`', r'<code>\1</code>', text)
    
    # Lists
    text = re.sub(r'^\* (.*)', r'<li>\1</li>', text, flags=re.MULTILINE)
    text = re.sub(r'^- (.*)', r'<li>\1</li>', text, flags=re.MULTILINE)
    # Wrap adjacent <li> in <ul>
    text = re.sub(r'(<li>.*</li>(?:\n<li>.*</li>)*)', r'<ul>\1</ul>', text)

    # Paragraphs (simple: double newline)
    text = re.sub(r'\n\n([^<].*)', r'\n\n<p>\1</p>', text)
    
    return text

def convert_md_to_html(md_text):
    html = md_text
    
    # Headers
    html = re.sub(r'^# (.*)', r'<h1>\1</h1>', html, flags=re.MULTILINE)
    
    # Sections (wrap ## content)
    parts = re.split(r'^## (.*)', html, flags=re.MULTILINE)
    new_body = parts[0]
    for i in range(1, len(parts), 2):
        title = parts[i]
        content = parts[i+1] if i+1 < len(parts) else ""
        content = process_markdown_body(content)
        new_body += f'<section class="section">\n<h2>{title}</h2>\n{content}\n</section>\n'
    
    return new_body

def main():
    try:
        data = json.load(sys.stdin)
    except Exception as e:
        # Silently fail if stdin is not JSON
        return
        
    file_path = data.get('tool_input', {}).get('file_path', '')
    
    if not file_path or not file_path.endswith('.md') or '/LEC' not in file_path:
        print("{}")
        return

    if not os.path.exists(file_path):
        print("{}")
        return
        
    with open(file_path, 'r') as f:
        md_text = f.read()
        
    css_path = os.path.join(os.getcwd(), '.gemini/skills/md-to-html-converter/assets/style.css')
    css_content = ""
    if os.path.exists(css_path):
        with open(css_path, 'r') as f:
            css_content = f.read()
            
    body_html = convert_md_to_html(md_text)
    
    # Extract Title if possible
    title_match = re.search(r'^# (.*)', md_text, re.MULTILINE)
    doc_title = title_match.group(1) if title_match else "Lecture Notes"
    
    final_html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{doc_title}</title>
    <style>
{css_content}
    </style>
</head>
<body>
    <div class="container">
{body_html}
    </div>
</body>
</html>"""

    html_path = file_path.replace('.md', '.html')
    with open(html_path, 'w') as f:
        f.write(final_html)
        
    # Return empty JSON to signal success to Gemini CLI
    print("{}")

if __name__ == "__main__":
    main()
