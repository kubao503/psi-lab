import flat


def test_flatten_dict():
    d = {"a": 1, "b": 2, "c": 3}
    expected = ["a", "b", "c", 1, 2, 3]
    assert flat.flatten_dict(d) == expected


def test_build_dict():
    flat_dict = ["a", "b", "c", 1, 2, 3]
    expected = {"a": 1, "b": 2, "c": 3}
    assert flat.build_dict(flat_dict) == expected
