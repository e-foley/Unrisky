
import requests
page_url = "https://www.predictit.org/api/marketdata/all/"
r = requests.get(page_url)  # create HTTP response object

# send a HTTP request to the server and save
# the HTTP response in a response object called r
with open("all.json",'w') as f:

    # Saving received content as a png file in
    # binary format

    # write the contents of the response (r.content)
    # to a new file in binary mode.
    f.write(r.text)
