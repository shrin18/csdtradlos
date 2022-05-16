/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pac_err_cal_impl.h"
#include <cstdio>
#include <iostream>
//#include <stdio>


namespace gr {
  namespace cdma {

    pac_err_cal::sptr
    pac_err_cal::make(unsigned long winsize, unsigned long cycsize, std::string pkt_num_tag, float erate)
    {
      return gnuradio::get_initial_sptr
        (new pac_err_cal_impl(winsize, cycsize, pkt_num_tag, erate));
    }

    /*
     * The private constructor
     */
    pac_err_cal_impl::pac_err_cal_impl(unsigned long winsize, unsigned long cycsize, std::string pkt_num_tag, float erate)
      : gr::block("pac_err_cal",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))),
              d_erate(erate)
              self.message_port_register_hier_out("d_erate")

              self.erate = erate
    {
      d_pkt_num_tag = pkt_num_tag;
      d_winsize = winsize;
      d_cycsize = cycsize;
      d_count_errs = 0;
      d_count_pktsent = 0;
      d_curnum = 0;

      d_prev = 0;
      d_erate = 0;

      message_port_register_in(pmt::mp("print"));
      set_msg_handler(pmt::mp("print"), boost::bind(&pac_err_cal_impl::print, this, _1));

      message_port_register_in(pmt::mp("errCal"));
      set_msg_handler(pmt::mp("errCal"), boost::bind(&pac_err_cal_impl::errCal, this, _1));
    }

    int 
    pac_err_cal_impl::general_work (int noutput_items,
                          gr_vector_int &ninput_items,
                          gr_vector_const_void_star &input_items,
                          gr_vector_void_star &output_items)
    {
    const float *in = (const float *) input_items[0];
    float *out = (float *) output_items[0];
    
    // Do <+signal processing+>
    // Tell runtime system how many input items we consumed on
    // each input stream.
    for (float d_erate = 0; d_erate < noutput_items; d_erate ++)
    {
      out[i] = get_error_rate(in[i]);
    consume_each (noutput_items);
    // Tell runtime system how many output items we produced.
    return noutput_items;
    }

    float pac_err_cal_impl::get_error_rate(const float &sample)
    if (erate) {
         erate = (float) d_count_errs / d_count_pktsent;
    }
    }


     // default implementation:  1:1
     void
     gr::block::forecast(int noutput_items,
                gr_vector_int &ninput_items_required)
    { 
    unsigned ninputs = ninput_items_required.size ();
    for(unsigned i = 0; i < ninputs; i++)
    ninput_items_required[i] = noutput_items;
    }

    /*
     * Our virtual destructor.
     */
    pac_err_cal_impl::~pac_err_cal_impl()
    {
    }

    void
    pac_err_cal_impl::print(pmt::pmt_t msg)
    {
      std::cout << "******* MESSAGE DEBUG  ********\n";
      pmt::print(msg);
      std::cout << "************************************\n";
    }

    void
    pac_err_cal_impl::errCal(pmt::pmt_t msg)
    {
      
      //pmt::pmt_t key0 = pmt::intern("cdma_packet_num");
      pmt::pmt_t key0 = pmt::intern(d_pkt_num_tag);
      if (pmt::dict_has_key (msg, key0))
      {

        pmt::pmt_t not_found;
        pmt::pmt_t val0 = pmt::dict_ref(msg,key0,not_found);
        unsigned long val = pmt::to_uint64(val0);

        //std::cout << "val: " << val << "\n";
        if (val < 0 || val >= d_cycsize)
        {
          //std::cout << "invalid index number\n";
          return;
        }

        d_curnum = d_curnum + 1; // # of packets stocked in the current window + 1

        //increment errs and packets sent
        if (val < d_prev) // a new cycle starts while the window hasn't been filled up
        { 
          d_count_errs = d_count_errs + d_cycsize + val - d_prev - 1;
          d_count_pktsent = d_count_pktsent + d_cycsize + val - d_prev;
        }
        else
        {
          d_count_errs = d_count_errs + val - d_prev - 1;
          d_count_pktsent = d_count_pktsent + val - d_prev;
        }
        std::cout << "E-RATE: " << d_erate << "\n";
        //terminate the window
        if (d_curnum >= d_winsize)
        {
          d_erate = (float) d_count_errs / d_count_pktsent; // key calculation
          
          //std::cout << "E-RATE: " << d_erate << "\t\t E-#: " << d_count_errs << "\n";          
          std::cout << "E-RATE: " << d_erate << "\n";
          std::cout << "Receving bits !" ;
          d_curnum = 0;
          d_count_errs = 0;
          d_count_pktsent = 0;
        }

        d_prev = val;     
      }
    }

  } /* namespace cdma */
} /* namespace gr */

