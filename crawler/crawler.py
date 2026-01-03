import requests
from bs4 import BeautifulSoup
import os
import time
import re
from urllib.parse import urljoin, urlparse

# --- CONFIGURATION ---
SEED_URLS = [
    "https://en.wikipedia.org/wiki/C++",
]
OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "../data")
MAX_PAGES = 50          # Increased limit since we are auto-discovering
DELAY_SECONDS = 1.0     # Politeness delay
ALLOWED_DOMAIN = "en.wikipedia.org" # Restrict crawler to this domain to prevent drifting

def get_safe_filename(title):
    """
    Converts a title into a safe filename.
    Removes characters invalid in Windows/Linux filenames: < > : " / \ | ? *
    """
    # Replace invalid characters with an underscore
    safe_title = re.sub(r'[<>:"/\\|?*]', '_', title)
    # Truncate to 100 chars to avoid filesystem limits
    return safe_title[:100].strip() + ".txt"

def save_document(url, title, text):
    """Saves the document using the Title as the filename."""
    filename = get_safe_filename(title)
    filepath = os.path.join(OUTPUT_DIR, filename)
    
    # Avoid overwriting if file exists (optional, maybe append a number)
    if os.path.exists(filepath):
        return None 

    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"{url}\n")
        f.write(f"{title}\n\n")
        f.write(text)
    
    return filename

def is_valid_url(url):
    """Checks if the URL belongs to the allowed domain and is not a file/image."""
    parsed = urlparse(url)
    # 1. Must be HTTP/HTTPS
    if parsed.scheme not in ['http', 'https']:
        return False
    # 2. Must match allowed domain (e.g., wikipedia.org)
    if ALLOWED_DOMAIN not in parsed.netloc:
        return False
    # 3. Skip special namespaces or files (basic heuristic)
    if any(x in parsed.path for x in [':', '.jpg', '.png', '.pdf']):
        return False
    return True

def crawl():
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)
    
    visited = set()
    queue = SEED_URLS.copy()
    count = 0

    print(f"--- Starting Crawler ---")
    print(f"Target Directory: {os.path.abspath(OUTPUT_DIR)}")

    while queue and count < MAX_PAGES:
        current_url = queue.pop(0)
        
        if current_url in visited:
            continue
        
        visited.add(current_url)

        try:
            print(f"[{count+1}/{MAX_PAGES}] Fetching: {current_url}")
            
            headers = {'User-Agent': 'MySearchEngineBot/1.0'}
            response = requests.get(current_url, headers=headers, timeout=5)
            
            if response.status_code != 200:
                print(f"   Skipped: Status {response.status_code}")
                continue

            soup = BeautifulSoup(response.text, 'html.parser')

            # --- 1. Extract Content ---
            page_title = soup.title.string.strip() if soup.title else "No Title"
            text_content = ""
            for p in soup.find_all('p'):
                text_content += p.get_text() + "\n"

            # --- 2. Save File ---
            saved_name = save_document(current_url, page_title, text_content)
            if saved_name:
                print(f"   Saved: {saved_name}")
                count += 1
            else:
                print(f"   Skipped: File already exists")

            # --- 3. Link Discovery (The "Frontier") ---
            # Find all <a href="..."> tags
            new_links_found = 0
            for link in soup.find_all('a', href=True):
                raw_href = link['href']
                
                # Resolve relative URLs (e.g., "/wiki/Java" -> "https://en.wikipedia.org/wiki/Java")
                full_url = urljoin(current_url, raw_href)
                
                # Remove fragments (e.g., #History) to avoid duplicate crawling
                full_url = full_url.split('#')[0]

                if full_url not in visited and full_url not in queue:
                    if is_valid_url(full_url):
                        queue.append(full_url)
                        new_links_found += 1
            
            print(f"   Found {new_links_found} new valid links.")

            time.sleep(DELAY_SECONDS)

        except Exception as e:
            print(f"   Error: {e}")

    print("--- Crawling Finished ---")

if __name__ == "__main__":
    crawl()