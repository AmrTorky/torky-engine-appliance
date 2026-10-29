import os
import sys

# Simple HTML-to-print format for quick PDF rendering or browser printing
with open("COMMERCIAL_AGREEMENT.md", "r", encoding="utf-8") as f:
    text = f.read()

html_content = f"""<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Commercial Agreement - Torky AT-ACE</title>
<style>
  body {{ font-family: 'Helvetica Neue', Arial, sans-serif; line-height: 1.6; padding: 40px; color: #222; max-width: 800px; margin: auto; }}
  h1 {{ color: #0284c7; border-bottom: 2px solid #0284c7; padding-bottom: 10px; font-size: 22px; }}
  h2 {{ color: #334155; margin-top: 25px; border-bottom: 1px solid #cbd5e1; padding-bottom: 5px; font-size: 16px; }}
  table {{ width: 100%; border-collapse: collapse; margin: 20px 0; }}
  th, td {{ border: 1px solid #cbd5e1; padding: 10px; text-align: left; font-size: 14px; }}
  th {{ background: #f1f5f9; }}
  pre {{ background: #f8fafc; padding: 10px; border-radius: 4px; font-size: 13px; }}
  .signatures {{ margin-top: 50px; display: flex; justify-content: space-between; }}
</style>
</head>
<body>
<pre style="white-space: pre-wrap; font-family: inherit;">{text}</pre>
</body>
</html>"""

with open("COMMERCIAL_AGREEMENT.html", "w", encoding="utf-8") as f:
    f.write(html_content)

print("[+] Generated COMMERCIAL_AGREEMENT.md and COMMERCIAL_AGREEMENT.html")
print("[*] To print to PDF directly, open COMMERCIAL_AGREEMENT.html in your browser and click Print -> Save as PDF.")
