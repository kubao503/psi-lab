def flatten_dict(d: dict) -> list:
    return [*d.keys(), *d.values()]


def build_dict(flat: list) -> dict:
    mid = len(flat) // 2
    return dict(zip(flat[:mid], flat[mid:]))
