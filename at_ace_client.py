#!/usr/bin/env python3
"""
AT-ACE Client SDK
Commercial Lightweight Python SDK for Torky Continuum Engine Microservice.
"""

import json
import urllib.request
import urllib.error
import numpy as np
from typing import List, Dict, Any, Union

class TorkyClient:
    def __init__(self, endpoint: str = "http://localhost:8080"):
        self.endpoint = endpoint.rstrip("/")

    def status(self) -> Dict[str, Any]:
        """Fetch engine health, governed records, and mathematical invariant bounds."""
        req = urllib.request.Request(f"{self.endpoint}/status", method="GET")
        with urllib.request.urlopen(req, timeout=5) as resp:
            return json.loads(resp.read().decode("utf-8"))

    def search(self, query_vector: Union[List[float], np.ndarray], top_k: int = 5) -> Dict[str, Any]:
        """
        Execute sub-millisecond Top-K vector search via closed-form angular governance.
        """
        if isinstance(query_vector, np.ndarray):
            query_vector = query_vector.tolist()

        if len(query_vector) != 64:
            raise ValueError(f"Expected 64-dimensional query vector, received {len(query_vector)}")

        payload = json.dumps({
            "top_k": top_k,
            "query_vector": query_vector
        }).encode("utf-8")

        req = urllib.request.Request(
            f"{self.endpoint}/search",
            data=payload,
            headers={"Content-Type": "application/json"},
            method="POST"
        )
        
        with urllib.request.urlopen(req, timeout=10) as resp:
            return json.loads(resp.read().decode("utf-8"))

if __name__ == "__main__":
    # Integration test with running container
    client = TorkyClient("http://localhost:8080")
    
    print("=" * 70)
    print("          AT-ACE Client SDK Live Verification Harness          ")
    print("=" * 70)
    
    # Check status
    st = client.status()
    print(f"Engine Target    : {st.get('engine')}")
    print(f"Governed Records : {st.get('governed_records'):,}")
    print(f"Compression Eff. : {st.get('compression_efficiency')}")
    print(f"Condition Bound  : kappa <= {st.get('invariant_kappa_max')}")
    print(f"Health Status    : {st.get('status')}")
    print("-" * 70)

    # Search with synthetic 64-D query
    print("[*] Dispatching 64-D query vector for Top-5 nearest matches...")
    random_query = np.random.randn(64).astype(np.float32)
    res = client.search(random_query, top_k=5)
    
    print(f"[+] Execution Latency : {res.get('execution_ms', res.get('execution_time_ms')):.3f} ms")
    print(f"[+] Records Scanned   : {res.get('scanned_records', 0):,}")
    print("Top Matches:")
    for i, match in enumerate(res.get("results", res.get("top_k", [])), 1):
        print(f"  Rank #{match.get('rank', i)} | Index: {match['record_index']:<7} | Cosine Score: {match['similarity_score']:.6f} | Mask: {match.get('metadata_mask')}")
    print("=" * 70)
