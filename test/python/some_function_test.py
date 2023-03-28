import duckdb

def test_some_function():
    conn = duckdb.connect('');
    conn.execute("SELECT some_function('Sam') as value;");
    res = conn.fetchall()
    assert(res[0][0] == "Some_function Sam 🐥");