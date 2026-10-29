import ctypes
import os
import numpy as np

_lib_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "libtorky_core.so")
if not os.path.exists(_lib_path):
    raise FileNotFoundError(f"Shared library not found at {_lib_path}")

_lib = ctypes.CDLL(_lib_path)

_lib.torky_create_streamer.argtypes = [ctypes.c_char_p, ctypes.c_uint64]
_lib.torky_create_streamer.restype = ctypes.c_void_p

_lib.torky_map_tile.argtypes = [ctypes.c_void_p, ctypes.c_uint64, ctypes.c_uint64]
_lib.torky_map_tile.restype = None

_lib.torky_write_packed.argtypes = [
    ctypes.c_void_p, ctypes.c_uint64,
    ctypes.c_uint8, ctypes.c_uint16, ctypes.c_uint16, ctypes.c_uint32
]
_lib.torky_write_packed.restype = None

_lib.torky_reconstruct_vector.argtypes = [
    ctypes.c_uint8, ctypes.c_uint16, ctypes.c_uint16, ctypes.c_uint32,
    ctypes.POINTER(ctypes.c_float)
]
_lib.torky_reconstruct_vector.restype = None

_lib.torky_search_tile_topk.argtypes = [
    ctypes.c_void_p, ctypes.c_uint64, ctypes.c_float, ctypes.c_int,
    ctypes.POINTER(ctypes.c_uint64), ctypes.POINTER(ctypes.c_float)
]
_lib.torky_search_tile_topk.restype = None

_lib.torky_destroy_streamer.argtypes = [ctypes.c_void_p]
_lib.torky_destroy_streamer.restype = None

class TorkyEngine:
    def __init__(self, storage_path, total_records=1000000):
        self.path = storage_path.encode('utf-8')
        self.total_records = total_records
        self.streamer = _lib.torky_create_streamer(self.path, total_records)
        if not self.streamer:
            raise RuntimeError("Failed to allocate physical storage backing file.")

    def stream_and_govern(self, records_batch, start_record_idx=0):
        count = len(records_batch)
        _lib.torky_map_tile(self.streamer, start_record_idx, count)
        for i, vec in enumerate(records_batch):
            r = np.linalg.norm(vec)
            theta = np.arctan2(vec[1], vec[0])
            if theta < 0:
                theta += 2.0 * np.pi
            mask = 0x2A
            q_r = int(np.clip(r * 1000.0, 0, 65535))
            q_theta = int((theta / (2.0 * np.pi)) * 65535)
            payload_hash = (start_record_idx + i + 1) * 987654321 % 4294967295
            _lib.torky_write_packed(self.streamer, i, mask, q_r, q_theta, payload_hash)

    def search_topk(self, query_vector, k=5, records_in_tile=100000):
        theta_q = float(np.arctan2(query_vector[1], query_vector[0]))
        if theta_q < 0:
            theta_q += 2.0 * np.pi
            
        out_indices = np.zeros(k, dtype=np.uint64)
        out_scores = np.zeros(k, dtype=np.float32)

        _lib.torky_search_tile_topk(
            self.streamer, records_in_tile, theta_q, k,
            out_indices.ctypes.data_as(ctypes.POINTER(ctypes.c_uint64)),
            out_scores.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
        )
        return out_indices, out_scores

    def reconstruct(self, mask, q_r, q_theta, payload_hash):
        out_vec = np.zeros(64, dtype=np.float32)
        _lib.torky_reconstruct_vector(
            mask, q_r, q_theta, payload_hash, 
            out_vec.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
        )
        return out_vec

    def close(self):
        if self.streamer:
            _lib.torky_destroy_streamer(self.streamer)
            self.streamer = None

def init(storage_path="data.torky", total_records=1000000):
    return TorkyEngine(storage_path, total_records)
