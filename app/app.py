import streamlit as st
import pandas as pd
import numpy as np

df = pd.read_csv("/var/log/packets_flow_rate_tracker/packets_flow_rate_tracker.log")
data = pd.DataFrame(df);
number_of_packets = data.columns[0]
times = data.columns[1]
print(type(data))
tab1, = st.tabs(["Chart"])
with tab1:
    st.line_chart(data, x=times, y=number_of_packets,  height=250)

