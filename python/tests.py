import unittest
import streambar
import tempfile
import pandas as pd 
import numpy as np
from io import StringIO
from numpy.testing import assert_array_equal
import os

class TestBars(unittest.TestCase):
    def setUp(self):
        # create a filename
        out = tempfile.NamedTemporaryFile(delete=False)
        self._fname = out.name
        out.close()

    def tearDown(self):
        # unlink the file again
        os.unlink(self._fname)

    def test_tick_small(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.tick("tests/small.tape", self._fname, size=2), 6)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [200, 200, 200, 200, 200, 100])
        assert_array_equal(df['buys'].values, [1, 2, 2, 2, 2, 1])
        assert_array_equal(df['vwap'].values, [101.25, 102.5, 102.5, 102.5, 102.5, 102.5])
        assert_array_equal(df['trades'].values, [2, 2, 2, 2, 2, 1])

    def test_tick_incremental(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.tick("tests/incremental.tape", self._fname, size=2), 6)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [300, 700, 1100, 1500, 1900, 1100])

    def test_volume_small(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.volume("tests/small.tape", self._fname, size=500), 3)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [500, 500, 100])
        assert_array_equal(df['buys'].values, [4, 5, 1])
        assert_array_equal(df['vwap'].values, [102, 102.5, 102.5])
        assert_array_equal(df['trades'].values, [5, 5, 1])

    def test_change_small(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.change("tests/small.tape", self._fname, size=100), 2)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [100, 1000])
        assert_array_equal(df['buys'].values, [0, 10])
        assert_array_equal(df['vwap'].values, [100, 102.5])
        assert_array_equal(df['trades'].values, [1, 10])
    
    def test_change_small_incremental_price(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.change("tests/incremental_price.tape", self._fname, size=500), 2)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [500, 600])
        assert_array_equal(df['buys'].values, [4, 6])
        assert_array_equal(df['vwap'].values, [102.4, 108])
        assert_array_equal(df['trades'].values, [5, 6])

    def test_volume_incremental(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.volume("tests/incremental.tape", self._fname, size=500), 8)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [600, 900, 600, 700, 800, 900, 1000, 1100])
        
    def test_time_small(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.time("tests/small.tape", self._fname, size=4), 3)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [400, 400, 300])
        assert_array_equal(df['buys'].values, [3, 4, 3])
        assert_array_equal(df['vwap'].values, [101.875, 102.5, 102.5])
        assert_array_equal(df['trades'].values, [4, 4, 3])

    def test_time_incremental(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.time("tests/incremental.tape", self._fname, size=4), 3)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [1000, 2600, 3000])

    def test_dollar_incremental(self):
        # should be 6 bars in total, with the last one being off
        self.assertEqual(streambar.dollar("tests/incremental.tape", self._fname, size=35000), 9)

        # open as a dataframe
        df = pd.read_csv(self._fname)
    
        # check the data
        assert_array_equal(df['volume'].values, [600, 400, 500, 600, 700, 800, 900, 1000, 1100])

    def test_invalid_file(self):
        # should be 6 bars in total, with the last one being off @todo typeerror is weird but works for now I guess
        self.assertRaises(TypeError, streambar.tick, "nx", "", size=123)

class TestPerformance(unittest.TestCase):
    def setUp(self):
        # create a filename
        out = tempfile.NamedTemporaryFile(delete=False)
        self._fname = out.name
        out.close()

    def tearDown(self):
        # unlink the file again
        os.unlink(self._fname)
    
    def test_performance(self):
        pass

if __name__ == '__main__':
    unittest.main()