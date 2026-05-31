import streamlit as st
import pandas as pd
import altair as alt
import numpy as np

df_chart = pd.read_csv("/var/log/packets_flow_rate_tracker/packets_flow_rate_tracker.csv", header = None, names=["packets","time"])

df_chart['time'] = pd.to_datetime(df_chart['time'], format='%H:%M:%S')

df_chart = alt.Chart(df_chart).mark_line().encode(
    x=alt.X(
        'time:T', 
        # 1. Increase the visible range boundary
        # scale=alt.Scale(domain=[0, 25]),
        axis=alt.Axis(
            format='%H:%M:%S',
            labelAngle=-45,       # Slants the text at an oblique 45-degree angle
            labelAlign='right',   # Aligns the top-right of the text to the tick mark
            labelBaseline='middle', # Keeps the text vertically centered relative to the angle
            tickCount=10
        )
    ),
    y='packets:Q'
).interactive()

st.title("FLOW RATE TRACKER")
tab1, = st.tabs(["Chart"])
with tab1:
    st.altair_chart(df_chart, height=250)
