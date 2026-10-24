import urllib.request, json

class TorkyClient:
    def __init__(self, endpoint="http://localhost:8080"):
        self.endpoint = endpoint.rstrip("/")
    def search(self, vector, top_k=5, mask=42, lexical_weight=0.25):
        req = urllib.request.Request(f"{self.endpoint}/search/hybrid", data=json.dumps({"top_k": top_k, "target_mask": mask, "lexical_weight": lexical_weight, "query_vector": vector}).encode(), headers={"Content-Type": "application/json"})
        return json.loads(urllib.request.urlopen(req).read().decode())
    def project(self, record_id, target_dim=512):
        return json.loads(urllib.request.urlopen(f"{self.endpoint}/project/{record_id}?dim={target_dim}").read().decode())
    def delete(self, record_id):
        req = urllib.request.Request(f"{self.endpoint}/delete", data=json.dumps({"record_index": record_id}).encode(), headers={"Content-Type": "application/json"})
        return json.loads(urllib.request.urlopen(req).read().decode())
